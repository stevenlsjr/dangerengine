/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSBEHAVIOR_H
#define DANGERENGINE_SLSBEHAVIOR_H

typedef struct slsBehavior slsBehavior;
typedef struct slsEntity slsEntity;
typedef struct slsAppState slsAppState;

/**
 * @brief A behavior "script" type modelled after Unity3D's monobehavior
 * class.
 * @detail External entity data and logic should be encapsulated in behavior's data field.
 */
struct slsBehavior {
  slsEntity *entity;

  void (*setup)(slsBehavior *self);
  void (*teardown)(slsBehavior *self);

  void (*update)(slsBehavior *self, slsAppState *state, double dt);

  void *data;
};

#endif //DANGERENGINE_SLSBEHAVIOR_H
