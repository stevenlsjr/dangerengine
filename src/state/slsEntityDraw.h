/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/8/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSENTITYDRAW_H
#define DANGERENGINE_SLSENTITYDRAW_H

#include "slsAppState.h"

typedef struct slsEntity slsEntity;

void sls_entity_draw(slsEntity *self, double dt, slsAppState *state);

#endif //DANGERENGINE_SLSENTITYDRAW_H
