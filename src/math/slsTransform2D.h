//
// Created by Steven on 8/16/15.
//

#ifndef DANGERENGINE_SLSTRANSFORM2D_H
#define DANGERENGINE_SLSTRANSFORM2D_H

#include "../slsutils.h"
#include "slsMatrixStack.h"
#include "slsTransform.h"
#include <kazmath/kazmath.h>

typedef  struct slsEntity slsEntity;


typedef struct slsTransform2D slsTransform2D;


struct slsTransform2D {
  kmVec2 pos;
  kmVec2 scale;
  float rot;

  int z_layer;

  slsTransform modelview;
};



kmMat4 *sls_transform2D_to_matrix(slsTransform2D *self, kmMat4 *out_opt) SLS_NONNULL(1);

bool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b) SLS_NONNULL(1, 2);
bool sls_transform2D_near(slsTransform2D const *a,
                          slsTransform2D const *b,
                          float epsilon) SLS_NONNULL(1, 2);

slsTransform2D *sls_transform2d_init(slsTransform2D *self) SLS_NONNULL(1);

kmMat4 *sls_transform2d_modelview(slsTransform2D *self, kmMat4 *out) SLS_NONNULL(1, 2);

kmVec2 sls_transform2d_local_to_world(slsTransform2D *self, kmVec2 const *in_opt) SLS_NONNULL(1);



kmVec2 sls_transform2d_world_to_local(slsTransform2D *self, kmVec2 const *in_opt) SLS_NONNULL(1);

float sls_transform2d_local_to_world_angle(slsTransform2D *self, float angle);
float sls_transform2d_world_to_local_angle(slsTransform2D *self, float angle);



#endif //DANGERENGINE_SLSTRANSFORM2D_H
