//
// Created by Steven on 8/6/15.
//

#include "slsEntity.h"
#include "slsEntityDraw.h"
#include <apr-1/apr_strings.h>
#include <apr-1/apr_strmatch.h>


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


  //printf("hello entity %s\n", self->name);

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

void sls_entity_display(slsEntity *self, slsAppState *state, double dt)
{
  apr_hash_index_t *itor;
  apr_pool_t *pool;

  slsComponentMask drawable = SLS_COMPONENT_MATERIAL |
                              SLS_COMPONENT_TEXTURE |
                              SLS_COMPONENT_MESH;
  if ((self->component_mask & drawable) == drawable) {

    sls_entity_draw(self, dt, state);
    //sls_log_info("%s will draw", self->name);
  }

  slsMatrixStack *mv = &state->model_view;
  sls_glmat_push(mv);

  if (self == state->root) {
    sls_glmat_identity(mv);
  }

  sls_glmat_scale(mv, (kmVec3){self->transform.scale.x, self->transform.scale.y, 1.0});

  sls_glmat_translate(mv, (kmVec3){self->transform.pos.x,
                                   self->transform.pos.y,
                                   0.0});

  sls_glmat_bind_top(mv, self->material->program, self->material->uniforms.model_view);
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
