/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#ifndef DANGERENGINE_SLSAPPSTATE_H
#define DANGERENGINE_SLSAPPSTATE_H

#include "slsInputState.h"
#include <renderer/slsmesh.h>
#include <renderer/slsrender.h>

typedef struct slsAppState slsAppState;
/**
 * @details
 * State for a simple 2d tank game. includes gpu resources as
 * well as game states. In future, this can be migrated to a swappable
 * game module.
 */
struct slsAppState {
  slsInputState *input;
  slsRendererGL *renderer;
  
};

slsAppState *sls_app_state_init(slsAppState *self, slsRendererGL *renderer);

slsAppState *sls_app_state_deinit(slsAppState *self);

void sls_app_state_update(slsAppState *self, double dt);

#endif // DANGERENGINE_SLSAPPSTATE_H
