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

  slsEntity *tilemap;

  slsMatrixStack work_stack;

} slsTankData;

slsEntity *sls_create_tank(slsAppState *state,
                           char const *name,
                           kmVec2 position,
                           float direction,
                           bool player_controlled,
                           slsMaterial *tex,
                           slsMaterial *barrel_tex,
                           slsShader *shader) SLS_NONNULL(1, 2, 6, 7, 8);

void sls_tankb_update(slsEntity *self, slsAppState *state, double dt) SLS_NONNULL(1, 2);

void sls_tank_turret_update(slsEntity *self, slsAppState *state, double dt);

#endif //DANGERENGINE_SLSTANK_H
