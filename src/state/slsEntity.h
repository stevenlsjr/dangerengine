//
// Created by Steven on 8/6/15.
//

#ifndef DANGERENGINE_SLSENTITY_H
#define DANGERENGINE_SLSENTITY_H

#include <renderer/slstexture.h>
#include <data-types/ptrarray.h>
#include <math/slsTransform2D.h>
#include <apr-1/apr_tables.h>
#include <apr-1/apr_hash.h>
#include <renderer/slsmesh.h>
#include <renderer/slsshader.h>
#include <math/slsCamera.h>
#include "slsAppState.h"
#include "slsBehavior.h"

typedef enum slsComponentMask {
  SLS_COMPONENT_NONE = 0,
  SLS_COMPONENT_MESH = 1 << 0,
  SLS_COMPONENT_MATERIAL = 1 << 1,
  SLS_COMPONENT_TEXTURE = 1 << 2,
  SLS_COMPONENT_BOUNDED = 1 << 3,
  SLS_COMPONENT_KINETIC = 1 << 4,
  SLS_COMPONENT_CAMERA = 1 << 5,
  SLS_COMPONENT_BEHAVIOR = 1 << 6
} slsComponentMask;


typedef struct slsEntity slsEntity;
typedef struct slsAppState slsAppState;


struct slsEntity {
  //---------------------------------constructor and destructor--------------------------------

  /**
   * @brief Component initializer
   */
  slsEntity *(*init)(slsEntity *self,
                     apr_pool_t *parent_pool,
                     slsComponentMask mask,
                     char const *name
  ) SLS_NONNULL(1, 2, 4);

  slsEntity *(*dtor)(slsEntity *self) SLS_NONNULL(1);

  //---------------------------------scene graph data---------------------------------------
  slsEntity *parent;
  apr_hash_t *children;

  //---------------------------------mandatory components-----------------------------------
  char *name;
  slsTransform2D transform;




  //---------------------------------polymorphic components---------------------------------

  slsComponentMask component_mask;

  slsMesh *mesh;
  bool mesh_is_owned; // flags whether entity manages mesh memory

  slsShader *shader;
  bool material_is_owned;

  slsTexture *texture;

  slsKinematic2D kinematic;

  slsCamera camera;

  slsBehavior behavior;

  apr_pool_t *pool;

  void *data;
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

void sls_entity_update(slsEntity *self,
                       slsAppState *state,
                       double dt);

void sls_entity_physicsupdate(slsEntity *self,
                              slsAppState *state,
                              double dt);


void sls_entity_display(slsEntity *self,
                        slsAppState *state,
                        double dt);


static inline slsEntity *sls_entity_new(
    apr_pool_t *parent_pool,
    slsComponentMask mask,
    char const *name)
{
  assert(name && parent_pool);
  return sls_entity_init((slsEntity *) calloc(1, sizeof(slsEntity)),
                         parent_pool, mask, name);
}

static inline void sls_entity_delete(slsEntity *self)
{
  assert(self);
  free(self->dtor(self));
}

#endif //DANGERENGINE_SLSENTITY_H
