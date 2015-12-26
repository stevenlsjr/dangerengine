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

/**
 * @brief applies entity transform to app's modelview stack, then binds to uniform
 */
void sls_drawable_transform(slsEntity *pEntity, slsAppState *pState, double dt);


#endif //DANGERENGINE_SLSENTITYDRAW_H
