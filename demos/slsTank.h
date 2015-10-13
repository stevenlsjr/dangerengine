/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/


#ifndef DANGERENGINE_SLSTANK_H
#define DANGERENGINE_SLSTANK_H

#include <state/slsEntity.h>

typedef struct slsTankData {
  slsEntity turret;
  slsEntity barrel;

  double acceleration;

  double max_speed;
  double rotational_speed;

} slsTankData;

slsEntity *sls_create_tank(slsAppState *state,
                           char const *name,
                           kmVec2 position,
                           float direction,
                           bool player_controlled,
                           slsTexture *tex,
                           slsShader *shader) SLS_NONNULL(1, 2, 6, 7);

void sls_tankb_update(slsBehavior *behavior, slsAppState *state, double dt) SLS_NONNULL(1, 2);


#endif //DANGERENGINE_SLSTANK_H
