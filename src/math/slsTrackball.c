/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/23/15, Steven
 *
 **/
#include "slsMathUtils.h"
#include "slsTrackball.h"

float sls_tb_project_to_sphere(float const radius, kmVec2 const *pVec2);

kmQuaternion *sls_trackball_calc_quat(kmQuaternion *out, float trackball_radius,
                                      float trackball_speed, kmVec2 const *p1,
                                      kmVec2 const *p2) {
  //sls_log_info("p1 %f %f, p2 %f %f", p1->x, p1->y, p2->x, p2->y);

  kmVec3 axis;
  kmVec3 _p1, _p2, dir;
  float phi;
  float t;
  float epsilon = 0.001;
  if (sls_vec2_near(p1, p2, epsilon)) {
    // zero rotation
    kmQuaternionIdentity(out);
    return out;
  }

  _p1 = (kmVec3){p1->x, p1->y, sls_tb_project_to_sphere(trackball_radius, p1)};
  _p2 = (kmVec3){p2->x, p2->y, sls_tb_project_to_sphere(trackball_radius, p2)};

  kmVec3Subtract(&dir, &_p1, &_p2);

  kmVec3Cross(&axis, &_p2, &_p1);

  t = kmVec3Length(&dir) / (2.0f * trackball_radius);
  t = fminf(fmaxf(-1.f, t), 1.f);
  phi = 2.0f * asinf(t) * trackball_speed;
  kmQuaternion tmp_a, tmp_b;
  kmQuaternionRotationAxisAngle(&tmp_a, &axis, phi);

  tmp_b = *out;

  return kmQuaternionMultiply(out, &tmp_a, &tmp_b);
  ;
}

float sls_tb_project_to_sphere(float const radius, kmVec2 const *vec) {

  float d, t, z;
  float x = vec->x, y = vec->y;

  d = sqrtf(x * x + y * y);
  if (d < radius * 0.70710678118654752440) { /* Inside sphere */
    z = sqrtf(radius * radius - d * d);
  } else { /* On hyperbola */
    t = radius / 1.41421356237309504880f;
    z = t * t / d;
  }
  return z;
}

void sls_trackball_set(slsTrackball *t, kmVec2 p1, kmVec2 p2) {
  sls_trackball_calc_quat(&t->rotation, t->radius, t->rotation_speed, &p1, &p2);

  kmMat4RotationQuaternion(&t->rotation_mat, &t->rotation);
}

slsTrackball *sls_trackball_init(slsTrackball *self, float radius,
                                 float rotation_speed) {
  self->radius = radius;
  self->rotation_speed = rotation_speed;
  kmQuaternionIdentity(&self->rotation);

  kmMat4Identity(&self->rotation_mat);

  return self;
}

void sls_trackball_drag(slsTrackball *ball, slsIPoint const *start_point,
                        slsIPoint const *second_point,
                        slsIPoint const *window_size) {

  float w = window_size->x, h = window_size->y;

  kmVec2 param_0 = {(2.f * start_point->x - w) / w,
                    (h - 2.f * start_point->y) / h};
  kmVec2 param_1 = {(2.f * second_point->x - w) / w,
                    (h - 2.f * second_point->y) / h};

  sls_trackball_set(ball, param_0, param_1);
}