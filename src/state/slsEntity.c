//
// Created by Steven on 8/6/15.
//


#include "slsEntity.h"
#include "slsEntityDraw.h"
#include <apr-1/apr_strings.h>
#include <apr-1/apr_strmatch.h>
#include <math/mathmacs.h>
#include <data-types/intrusivelist.h>
#include <slscontext.h>

//---------------------------------constructor/destructor---------------------------------------

struct slsEntity_p {
  SLS_INTRUSIVE_FIELDS(slsEntity, list_node);
  bool skip_update;
};


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

  sls_check(apr_pool_create(&self->pool, parent_pool) == APR_SUCCESS, "pool creation failed");
  self->component_mask |= mask;

  self->children = apr_hash_make(self->pool);
  sls_checkmem(self->children);

  self->name = apr_pstrdup(self->pool, name);
  sls_checkmem(self->name);

  self->transform = (slsTransform2D) {
      .pos = (kmVec2) {0.0, 0.0},
      .scale = (kmVec2) {1.0, 1.0},
      .rot = 0.0,
      .entity = self
  };

  self->priv = apr_pcalloc(self->pool, sizeof(slsEntity_p));

  self->priv->list_node.next = self;
  self->priv->list_node.prev = self;

  sls_checkmem(self->priv);
  self->priv->skip_update = false;

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
  if (self->pool) {
    apr_pool_destroy(self->pool);
  }

  return self;

  error:
  assert(0);
  return NULL;
}

//-----------------------------entity heirarchy manipulation--------------------------------

void sls_entity_linkchild(slsEntity *self, apr_pool_t *p, slsEntity *child)
{
  // call setup behavior

  if ((self->component_mask & SLS_COMPONENT_BEHAVIOR) == SLS_COMPONENT_BEHAVIOR &&
      self->behavior.setup) {
    self->behavior.setup(self);
  }

  slsEntity *next = self->il.next;
  child->il.next = next;
  child->il.prev = self;
  self->il.next = child;
  if (next) {
    next->il.prev = child;
  }

  if (child->children) {
    for (apr_hash_index_t *itor = apr_hash_first(p, child->children);
         itor != NULL;
         itor = apr_hash_next(itor)) {

      slsEntity *cc;
      slsEntity *last = child;
      apr_hash_this(itor, NULL, NULL, (void **) &cc);
      if (cc) {
        sls_entity_linkchild(last, p, cc);
        last = cc;
      }
    }
  }
}

void sls_entity_unlink(slsEntity *self, apr_pool_t *p)
{
  // call teardown behavior

  if ((self->component_mask & SLS_COMPONENT_BEHAVIOR) == SLS_COMPONENT_BEHAVIOR &&
      self->behavior.teardown) {
    self->behavior.teardown(self);
  }

  slsEntity *prev = self->il.prev;
  slsEntity *next = self->il.next;
  if (prev) {
    prev->il.next = next;

    if (next) {
      next->il.prev = prev;
    }

  }
  self->il.prev = NULL;
  self->il.next = NULL;


  if (self->children) {
    for (apr_hash_index_t *itor = apr_hash_first(p, self->children);
         itor != NULL;
         itor = apr_hash_next(itor)) {

      slsEntity *cc;
      apr_hash_this(itor, NULL, NULL, (void **) &cc);
      if (cc) {
        sls_entity_unlink(cc, p);
      }
    }
  }
}

void sls_entity_addchild(slsEntity *self, slsEntity *child)
{
  sls_check(self->children, "self->children is NULL");
  sls_check(child->name, "child must have name");
  sls_check(self != child, "entity %s is adding itself as a child", self->name);

  for (slsEntity *e = self->parent; e != NULL; e = e->parent) {
    sls_check(e != child, "adding one of %s's ancestors as a child", self->name);
    sls_check(e != self, "found a cycle in %s's heirarchy tree!", self->name);
  }

  if (child->parent) {
    sls_entity_removechild(child->parent, child);
  }
  child->parent = self;
  child->priv->skip_update = true;

  apr_hash_set(self->children, child->name, APR_HASH_KEY_STRING, child);


  apr_pool_t *tmp;
  slsEntity *last = child;
  sls_checkmem(apr_pool_create(&tmp, NULL) == APR_SUCCESS);

  sls_entity_linkchild(self, tmp, child);


  apr_pool_destroy(tmp);

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
  apr_pool_t *tmp = NULL;

  sls_check(self == child->parent, "self is not child's parent");

  sls_check(sls_entity_findchild_byname(self, child->name) == child,
            "child is not member of parent's child table");

  apr_hash_set(self->children, child->name, APR_HASH_KEY_STRING, NULL);

  apr_pool_create(&tmp, self->pool);
  sls_entity_unlink(child, tmp);


  apr_pool_destroy(tmp);


  return child;
  error:
  assert(0);
  return child;
}

//---------------------------------entity child search---------------------------------------

slsEntity *sls_entity_findchild_reference(slsEntity *self,
                                          slsEntity const *child, apr_pool_t *pool)
{
  assert(child->name);

  slsEntity *result = NULL;

  for (apr_hash_index_t *itor = apr_hash_first(pool, self->children);
       itor != NULL;
       itor = apr_hash_next(itor)) {
    slsEntity *e;
    apr_hash_this(itor, NULL, NULL, (void **) &e);
    if (e == child) {
      result = e;
      break;
    }
  }

  return result;
}

slsEntity *sls_entity_findnamed(slsEntity *self, char const *name)
{
  size_t max_strlen = 10000;

  for (slsEntity *itor = self->il.next;
       itor != NULL && itor != self;
       itor = itor->il.next) {
    if (strncmp(itor->name, name, max_strlen) == 0) {
      return itor;
    }

  }

  return NULL;

}

slsEntity *sls_entity_getroot(slsEntity *self)
{
  slsEntity *itor = self;
  while (itor->parent != NULL) {
    itor = itor->parent;
  }
  return itor;
}

//---------------------------------game loop functions---------------------------------------
void sls_entity_update(slsEntity *self, slsAppState *state, double dt)
{
  if (self->priv && self->priv->skip_update) {
    self->priv->skip_update = false;
    return;
  }
  apr_hash_index_t *itor;
  apr_pool_t *pool = state->context->tmp_pool;


  bool has_behavior =
      (self->component_mask & SLS_COMPONENT_BEHAVIOR) ==
      SLS_COMPONENT_BEHAVIOR;

  if (has_behavior && self->behavior.update) {
    self->behavior.entity = self;
    self->behavior.update(self, state, dt);
  }

  if ((self->component_mask & SLS_COMPONENT_KINETIC) == SLS_COMPONENT_KINETIC) {
    sls_entity_physicsupdate(self, state, dt);
  }

  /*
  for (itor = apr_hash_first(pool, self->children);
       itor;
       itor = apr_hash_next(itor)) {

    slsEntity *child;
    apr_hash_this(itor, NULL, NULL, (void **) &child);
    if (child) { sls_entity_update(child, state, dt); }

  }
   */


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


  float linear_drag = fminf(1.0, km->linear_drag);
  linear_drag = fmaxf(0.0, linear_drag);

  // linear motion
  //kmVec2Normalize(&drag_vec, &km->velocity);
  drag_vec = km->velocity;

  kmVec2Scale(&drag_vec,
              &drag_vec,
              (float) (linear_drag));

  drag_vec.x = fabsf(drag_vec.x) < fabsf(km->velocity.x) ?
               drag_vec.x : km->velocity.x;

  drag_vec.y = fabsf(drag_vec.y) < fabsf(km->velocity.y) ?
               drag_vec.y : km->velocity.y;

  kmVec2Subtract(&km->velocity, &self->kinematic.velocity, &drag_vec);


  kmVec2Scale(&motion, &km->velocity, (float) dt);

  kmVec2Add(&self->transform.pos, &self->transform.pos, &motion);

}


void sls_entity_display(slsEntity *self, slsAppState *state, double dt)
{
  apr_hash_index_t *itor;
  apr_pool_t *pool = state->context->tmp_pool;


  slsMatrixStack *mv = &state->model_view;

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


  if ((self->component_mask & SLS_COMPONENT_DRAWABLE) == SLS_COMPONENT_DRAWABLE) {

    sls_entity_draw(self, dt, state);
    sls_drawable_transform(self, state, dt);

    //sls_log_info("%s will draw", self->name);
  }

  for (itor = apr_hash_first(pool, self->children);
       itor;
       itor = apr_hash_next(itor)) {

    slsEntity *child;

    apr_hash_this(itor, NULL, NULL, (void **) &child);
    assert(child != self);

    if (child) { sls_entity_display(child, state, dt); }

  }

  sls_matrix_stack_pop(mv, NULL);


  return;
  error:
  return;

}


