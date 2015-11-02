/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/22/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSPHYSICS_H
#define DANGERENGINE_SLSPHYSICS_H

#include <kazmath/kazmath.h>
#include <chipmunk/chipmunk.h>

typedef  struct slsKinematic2D slsKinematic2D;


struct slsKinematic2D {
  float mass;
  kmVec2 velocity;

  /**
   * @brief coefficient of friction for linear motion
   */
  float linear_drag;

  /**
   * @brief coefficient of friction for rotational motion
   */
  float rotational_drag;

  float rotational_inertia;
  float rotational_speed;


  
};

#endif //DANGERENGINE_SLSPHYSICS_H
