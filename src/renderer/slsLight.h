/**
 * @file ${FILE}
 * @brief
 * @detail This Header describes 3 types for defining lights in a scene:
 * a simple "Light" struct for defining the components of a single light source,
 * a "Light batch" structure organized as with *struct of arrays* design for efficiently
 * light data to the GPU, and a "light view" for viewing modifying a single light source
 * within a batch.
 *
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSLIGHT_H
#define DANGERENGINE_SLSLIGHT_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <slsutils.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct slsLight slsLight;

struct slsLight {
  kmVec3 ambient_product;
  kmVec3 diffuse_product;
  kmVec3 specular_product;
  kmVec4 light_position;
  kmMat4 light_modelview;
};


typedef struct slsLightBatch slsLightBatch;

struct slsLightBatch {
  size_t n_lights;
  size_t max_lights;
  kmVec3 *ambient_products;
  kmVec3 *diffuse_products;
  kmVec3 *specular_products;
  kmVec4 *light_positions;
  kmMat4 *light_modelviews;
};


typedef struct slsLightView slsLightView;

struct slsLightView {
  kmVec3 *ambient_product;
  kmVec3 *diffuse_product;
  kmVec3 *specular_product;
  kmVec4 *light_position;
  kmMat4 *light_modelview;
};

slsLightBatch *sls_lightbatch_init(slsLightBatch *self,
                                   size_t max_lights,
                                   slsLight const *lights,
                                   size_t n_lights) SLS_NONNULL(1);

slsLightBatch *sls_lightbatch_dtor(slsLightBatch *self) SLS_NONNULL(1);

void sls_lightbatch_set_light(slsLightBatch *self,
                              size_t idx, slsLight
                              const *light,
                              bool active) SLS_NONNULL(1, 3);

slsLight *sls_lightbatch_get_light(slsLightBatch *self,
                                   size_t idx,
                                   slsLight *out) SLS_NONNULL(1, 3);

slsLightView *sls_lightbatch_get_lightview(slsLightBatch *self,
                                           size_t idx,
                                           slsLightView *out) SLS_NONNULL(1, 3);

#endif //DANGERENGINE_SLSLIGHT_H
