/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/
#include "slsCamera.h"
#include <math.h>
#include "math-types.h"

void sls_camera_resize(slsCamera *self, int x, int y)
{


  float aspect = x/(float) y;
  if (self->is_perspective) {
    kmMat4PerspectiveProjection(&self->frustrum,
                                self->perspective.fov,
                                aspect,
                                self->perspective.near,
                                self->perspective.far);
  } else {
    float left = -1.0f,
        right = 1.0f,
        up = 1.0f,
        down = -1.0f;
    if (x > y) {
      left *= aspect;
      right *= aspect;
    } else {
      up *= 1/aspect;
      down *= 1/aspect;
    }
    float h_width = !isnan(self->ortho.width) && self->ortho.width != 0?
                    self->ortho.width/2.f:
                    x/2.f;
    float h_height = !isnan(self->ortho.height) && self->ortho.height != 0?
                    self->ortho.height/2.f:
                    x/2.f;

    assert(h_height != NAN);
    assert(h_width != NAN);

    kmMat4OrthographicProjection(&self->frustrum,
                                 left * h_width, right * h_width,
                                 down * h_height, up * h_height,
                                 self->ortho.near, self->ortho.far);
  }

}

slsCamera *sls_camera_init(slsCamera *self)
{
  self->is_perspective = false;

  sls_transform2d_init(&self->transform);


  self->ortho.far = (float)-50.0;
  self->ortho.near = (float)50.0;

  return self;
}
