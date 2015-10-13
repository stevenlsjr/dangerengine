//
// Created by Steven on 8/6/15.
//

#include "slsEntity.h"
#include "slsEntityDraw.h"
#include <apr-1/apr_strings.h>
#include <apr-1/apr_strmatch.h>
#include <math/mathmacs.h>


slsEntity *sls_entity_dtor(slsEntity *self) SLS_NONNULL(1);


slsEntity const *sls_entity_class()
{
  static slsEntity klass = {
      .dtor = sls_entity_dtor,
      .init = sls_entity_init,

      .parent = NULL,
      .children = NULL,

      .component_mask = SLS_COMPONENT_NONE,
      .name = NULL,
      .transform = (slsTransform2D) {},
  };

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"
  return &klass;
#pragma clang diagnostic pop
}

slsEntity *sls_entity_init(slsEntity *self,
                           apr_pool_t *parent_pool,
                           slsComponentMask mask,
                           char const *name)
{
  *self = *sls_entity_class();

  apr_status_t pool_s = apr_pool_create(&self->pool, parent_pool);

  sls_check(pool_s == APR_SUCCESS, "pool creation failed");
  self->component_mask |= mask;


  self->children = apr_hash_make(self->pool);
  sls_checkmem(self->children);

  self->name = apr_pstrdup(self->pool, name);
  sls_checkmem(self->name);


  self->transform = (slsTransform2D) {
      .pos = (kmVec2) {0.0, 0.0},
      .scale = (kmVec2) {1.0, 1.0},
      .rot = 0.0
  };

  return self;
  error:
  if (self && self->dtor) {
    return self->dtor(self);
  } else {
    return NULL;
  }
}

slsEntity *sls_entity_dtor(slsEntity *self)
{
  sls_checkmem(self->pool);
  if (self->parent) {
    sls_entity_removechild(self->parent, self);
  }
  if (self->children) {

    for (apr_hash_index_t *idx = apr_hash_first(self->pool, self->children);
         idx; idx = apr_hash_next(idx)) {
      slsEntity *child = NULL;
      apr_hash_this(idx, NULL, NULL, (void **) &child);
      if (child) {
        child->dtor(child);
      }
    }
  }

  apr_pool_destroy(self->pool);

  return self;

  error:
  assert(0);
  return NULL;
}

void sls_entity_addchild(slsEntity *self, slsEntity *child)
{
  sls_check(self->children, "self->children is NULL");
  child->parent = self;

  apr_hash_set(self->children, child->name, APR_HASH_KEY_STRING, child);

  return;
  error:
  assert(0);
  return;

}

slsEntity *sls_entity_findchild_byname(slsEntity *self, char const *child_name)
{
  assert(self->children);
  return apr_hash_get(self->children, child_name, APR_HASH_KEY_STRING);
}

slsEntity *sls_entity_removechild(slsEntity *self, slsEntity *child)
{
  sls_check(self == child->parent, "self is not child's parent");

  sls_check(sls_entity_findchild_byname(self, child->name) == child,
            "child is not member of parent's child table");

  apr_hash_set(self->children, child->name, APR_HASH_KEY_STRING, NULL);

  return child;
  error:
  assert(0);
  return child;
}

slsEntity *sls_entity_findchild_reference(slsEntity *self, slsEntity const *child)
{
  assert(child->name);
  return sls_entity_findchild_byname(self, child->name);
}

slsEntity *sls_entity_getroot(slsEntity *self)
{
  slsEntity *itor = self;
  while (itor->parent != NULL) {
    itor = itor->parent;
  }
  return itor;
}

void sls_entity_update(slsEntity *self, slsAppState *state, double dt)
{
  apr_hash_index_t *itor;
  apr_pool_t *pool;

  sls_checkmem(apr_pool_create(&pool, self->pool) == APR_SUCCESS);


  bool has_behavior =
      (self->component_mask & SLS_COMPONENT_BEHAVIOR) ==
      SLS_COMPONENT_BEHAVIOR;

  if (has_behavior && self->behavior.update) {
    self->behavior.entity = self;
    self->behavior.update(&self->behavior, state, dt);
  }

  if ((self->component_mask & SLS_COMPONENT_KINETIC) ==
      SLS_COMPONENT_KINETIC) {
    sls_entity_physicsupdate(self, state, dt);
  }

  for (itor = apr_hash_first(pool, self->children);
       itor;
       itor = apr_hash_next(itor)) {

    slsEntity *child;
    apr_hash_this(itor, NULL, NULL, (void **) &child);
    if (child) { sls_entity_update(child, state, dt); }

  }


  return;
  error:
  return;

}

void sls_entity_physicsupdate(slsEntity *self,
                              slsAppState *state,
                              double dt)
{

  slsKinematic2D *km = &self->kinematic;
  kmVec2 drag_vec, motion;

  float

  // linear motion
  kmVec2Normalize(&drag_vec, &km->velocity);

  kmVec2Scale(&drag_vec,
              &drag_vec,
              (float) (km->linear_drag));

  drag_vec.x = fabsf(drag_vec.x) < fabsf(km->velocity.x)?
      drag_vec.x: km->velocity.x;

  drag_vec.y = fabsf(drag_vec.y) < fabsf(km->velocity.y)?
               drag_vec.y: km->velocity.y;

  kmVec2Subtract(&km->velocity, &self->kinematic.velocity, &drag_vec);



  kmVec2Scale(&motion, &km->velocity, (float) dt);

  kmVec2Add(&self->transform.pos, &self->transform.pos, &motion);

  // rotational motion

  //debugging
  if (state->input.key_space) {
    sls_log_info("entity %s\n"
                     "linear drag %f\n"
                     "drag vec %f %f\n"
                     "velocity %f %f",
                 self->name,
                 dt * self->kinematic.linear_drag,
                 drag_vec.x, drag_vec.y,
                 self->kinematic.velocity.x, self->kinematic.velocity.y);
  }

}

void sls_entity_display(slsEntity *self, slsAppState *state, double dt)
{
  apr_hash_index_t *itor;
  apr_pool_t *pool;

  slsComponentMask drawable = SLS_COMPONENT_MATERIAL |
                              SLS_COMPONENT_TEXTURE |
                              SLS_COMPONENT_MESH;

  slsMatrixStack *mv = &state->model_view;


  if (self == state->root) {
    sls_matrix_glreset(mv);
    sls_matrix_glidentity(mv);
  }


  sls_matrix_glpush(mv);
  sls_matrix_glidentity(mv);

  kmMat4 local_transform;
  sls_transform2D_to_matrix(&local_transform, &self->transform);

  kmMat4 *top = sls_matrix_stack_peek(mv);
  if (top) {
    self->transform.model_view = *top;

    kmMat4Multiply(top, &self->transform.model_view, &local_transform);
    self->transform.model_view = *top;
  }


  if ((self->component_mask & drawable) == drawable) {

    sls_entity_draw(self, dt, state);
    sls_drawable_transform(self, state, dt);

    //sls_log_info("%s will draw", self->name);
  }

  sls_checkmem(apr_pool_create(&pool, self->pool) == APR_SUCCESS);

  for (itor = apr_hash_first(pool, self->children);
       itor;
       itor = apr_hash_next(itor)) {

    slsEntity *child;
    apr_hash_this(itor, NULL, NULL, (void **) &child);
    if (child) { sls_entity_display(child, state, dt); }

  }

  sls_matrix_stack_pop(mv, NULL);


  return;
  error:
  return;

}
