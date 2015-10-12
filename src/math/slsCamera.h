/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSCAMERA_H
#define DANGERENGINE_SLSCAMERA_H

#include <stdbool.h>
#include "slsTransform2D.h"

typedef struct slsCamera slsCamera;

struct slsCamera {
  bool is_perspective;
  struct {
    float fov;
    float near;
    float far;
  } perspective;

  struct {
    float near;
    float far;
  } ortho;

  kmMat4 frustrum;

  slsTransform2D transform;
};

void sls_camera_resize(slsCamera *camera, int x, int y) SLS_NONNULL(1);
slsCamera *sls_camera_init(slsCamera *camera) SLS_NONNULL(1);



#endif //DANGERENGINE_SLSCAMERA_H
