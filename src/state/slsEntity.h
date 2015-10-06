//
// Created by Steven on 8/6/15.
//

#ifndef DANGERENGINE_SLSENTITY_H
#define DANGERENGINE_SLSENTITY_H

#include <renderer/slstexture.h>
#include <data-types/ptrarray.h>
#include <math/slsTransform2D.h>
#include <renderer/slsSprite.h>
#include <apr-1/apr_tables.h>
#include <apr-1/apr_hash.h>
#include <renderer/slsmesh.h>
#include "slsAppState.h"

typedef enum slsComponentMask {
  SLS_COMPONENT_NONE = 0,
  SLS_COMPONENT_TRANSFORM = 1 << 1,
  SLS_COMPONENT_SPRITE = 1 << 2,
  SLS_COMPONENT_STATEACCESS = 1 << 3,
  SLS_COMPONENT_MESH = 1 << 4,
  SLS_COMPONENT_TEXTURE = 1 << 5
} slsComponentMask;


typedef struct slsEntity slsEntity;
typedef struct slsAppState slsAppState;


struct slsEntity {
  slsEntity *(*init)(slsEntity *self,
                     apr_pool_t *parent_pool,
                     slsComponentMask mask,
                     char const *name
  ) SLS_NONNULL(1, 2, 4);

  slsEntity *(*dtor)(slsEntity *self) SLS_NONNULL(1);

  slsEntity *parent;
  apr_hash_t *children;
  char *name;


  slsComponentMask component_mask;
  slsTransform2D transform;

  slsAppState *state;

  slsMesh *mesh;
  slsTexture *texture;

  apr_pool_t *pool;

};

slsEntity const *sls_entity_class();

slsEntity *sls_entity_init(slsEntity *self,
                           apr_pool_t *parent_pool,
                           slsComponentMask mask,
                           char const *name) SLS_NONNULL(1, 2, 4);

void sls_entity_addchild(slsEntity *self,
                         slsEntity *child) SLS_NONNULL(1, 2);

slsEntity *sls_entity_findchild_byname(slsEntity *self,
                                       char const *child_name) SLS_NONNULL(1, 2);

slsEntity *sls_entity_removechild(slsEntity *self,
                                  slsEntity *child) SLS_NONNULL(1, 2);

slsEntity *sls_entity_findchild_reference(slsEntity *self,
                                          slsEntity const *child) SLS_NONNULL(1, 2);

slsEntity *sls_entity_getroot(slsEntity *self) SLS_NONNULL(1);


static inline slsEntity *sls_entity_new(
    apr_pool_t *parent_pool,
    slsComponentMask mask,
    char const *name)
{
  assert(name && parent_pool);
  return sls_entity_init(calloc(1, sizeof(slsEntity)), parent_pool, mask, name);
}

static inline void sls_entity_delete(slsEntity *self)
{
  assert(self);
  free(self->dtor(self));
}

#endif //DANGERENGINE_SLSENTITY_H
