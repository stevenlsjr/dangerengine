/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/23/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSTRACKBALL_H
#define DANGERENGINE_SLSTRACKBALL_H

#include <kazmath/kazmath.h>

typedef struct slsTrackball slsTrackball;

struct slsTrackball {
  kmQuaternion rotation;
  float radius;
  float rotation_speed;

  kmMat4 rotation_mat;
};

kmQuaternion *sls_trackball_calc_quat(kmQuaternion *out, float trackball_radius, float trackball_speed,
                                      kmVec2 const *p1, kmVec2 const *p2);

void sls_trackball_set(slsTrackball *t, kmVec2 p1, kmVec2 p2);

slsTrackball *sls_trackball_init(slsTrackball *self, float radius, float rotation_speed);

static inline
slsTrackball *sls_trackball_init_default(slsTrackball *self)
{
  return sls_trackball_init(self, 0.8, 2.0);
}


#endif //DANGERENGINE_SLSTRACKBALL_H
