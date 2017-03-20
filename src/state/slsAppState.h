/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#ifndef DANGERENGINE_SLSAPPSTATE_H
#define DANGERENGINE_SLSAPPSTATE_H

#include <renderer/slsmesh.h>
#include <renderer/slsrender.h>
#include "slsInputState.h"

typedef struct slsAppState slsAppState;

struct slsAppState {
  slsInputState *input;
  slsRendererGL *renderer;

  // TODO: Scene description object

  slsShader mesh_shader;
  slsMesh mesh;
};

slsAppState *sls_app_state_init(slsAppState *self, slsRendererGL *renderer);
slsAppState *sls_app_state_deinit(slsAppState *self);

void sls_app_state_update(slsAppState *self, double dt);

#endif // DANGERENGINE_SLSAPPSTATE_H
