/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include "slsLight.h"

slsLightBatch *sls_lightbatch_init(slsLightBatch *self, size_t max_lights,
                                   slsLight const *lights, size_t n_lights)
{
  *self = (slsLightBatch){};
  self->ambient_products = calloc(max_lights + 1, sizeof(kmVec3));
  self->diffuse_products = calloc(max_lights + 1, sizeof(kmVec3));
  self->specular_products = calloc(max_lights + 1, sizeof(kmVec3));
  self->light_positions = calloc(max_lights + 1, sizeof(kmVec4));
  self->light_modelviews = calloc(max_lights + 1, sizeof(kmMat4));

  self->max_lights = max_lights;

  if (lights) {
    self->n_lights = n_lights;
    for (int i = 0; i < n_lights; ++i) {
      sls_lightbatch_set_light(self, i, lights + i, true);
    }

  } else {
    self->n_lights = 0;
  }

  return self;
}

void sls_lightbatch_set_light(slsLightBatch *self, size_t idx,
                              slsLight const *light, bool active)
{

  self->ambient_products[idx] = light->ambient_product;
  self->specular_products[idx] = light->specular_product;
  self->diffuse_products[idx] = light->diffuse_product;
  self->light_positions[idx] = light->light_position;
  self->light_modelviews[idx] = light->light_modelview;

  if (self->n_lights <= idx) {
    self->n_lights = idx + 1;
  }
}

slsLight *sls_lightbatch_get_light(slsLightBatch *self, size_t idx,
                                   slsLight *out)
{
  return NULL;
}

slsLightBatch *sls_lightbatch_dtor(slsLightBatch *self)
{
  void *mem[] = { self->ambient_products, self->specular_products,
                  self->diffuse_products, self->light_positions,
                  self->light_modelviews };
  for (size_t i = 0; i < sizeof(mem) / sizeof(void *); ++i) {
    if (mem[i]) {
      free(mem[i]);
    }
  }
  return self;
}

slsLightView *sls_lightbatch_get_lightview(slsLightBatch *self, size_t idx,
                                           slsLightView *out)
{
  if (idx >= self->n_lights) {
    sls_log_err("index overflow: idx=%lu n_lights=%lu", idx, self->n_lights);
    return NULL;
  }

  out->ambient_product = self->ambient_products + idx;
  out->specular_product = self->specular_products + idx;
  out->diffuse_product = self->diffuse_products + idx;
  out->light_position = self->light_positions + idx;
  out->light_modelview = self->light_modelviews + idx;

  return out;
}
