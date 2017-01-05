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

typedef struct slsAppState slsAppState;


struct slsAppState {
  slsInputState *input;
};



slsAppState *sls_app_state_init(slsAppState *self);
slsAppState *sls_app_state_deinit(slsAppState *self);

void sls_app_state_update(slsAppState *self, double dt);


#endif //DANGERENGINE_SLSAPPSTATE_H
