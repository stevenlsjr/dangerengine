/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#include <math/math-types.h>
#include "slsAppState.h"

slsAppState *sls_app_state_init(slsAppState *self)
{
  *self = (slsAppState){};
  self->input = sls_inputstate_new();
  sls_checkmem(self->input);

  sls_lightbatch_init(&self->lights, 8, NULL, 0);
  slsMesh *mesh = sls_mesh_square(&self->mesh);
  sls_checkmem(mesh);

  return self;

error:
  if (self) {
    return sls_app_state_deinit(self);
  } else {
    return NULL;
  }
}

slsAppState *sls_app_state_deinit(slsAppState *self)
{
  if (self->input) {
    sls_inputstate_delete(self->input);
  }

  return self;
}

void sls_app_state_update(slsAppState *self, double dt)
{
  sls_inputstate_update(self->input, dt);
}