/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/24/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSTRANSFORM_H
#define DANGERENGINE_SLSTRANSFORM_H

#include <kazmath/kazmath.h>
#include <slsutils.h>

typedef struct slsTransform slsTransform;

typedef struct slsTransform_p slsTransform_p;

struct slsTransform_p {
  kmVec3 position;
  kmVec3 scale;
  kmQuaternion rotation;
};

struct slsTransform {
  kmMat4 matrix;

  slsTransform_p *priv;
};

slsTransform *sls_transform_init(slsTransform *self) SLS_NONNULL(1);
slsTransform *sls_transform_dtor(slsTransform *self) SLS_NONNULL(1);

slsTransform *sls_transform_set_position(slsTransform *self, kmVec3 const *position) SLS_NONNULL(1, 2);
slsTransform *sls_transform_set_scale(slsTransform *self, kmVec3 const *scale) SLS_NONNULL(1, 2);
slsTransform *sls_transform_set_rotation(slsTransform *self, kmQuaternion const *rotation) SLS_NONNULL(1, 2);

kmVec3 sls_transform_get_position(slsTransform *self) SLS_NONNULL(1);
kmVec3 sls_transform_get_scale(slsTransform *self) SLS_NONNULL(1);
kmQuaternion sls_transform_get_rotation(slsTransform *self) SLS_NONNULL(1);

void sls_transform_build_matrix(slsTransform *self) SLS_NONNULL(1);




#endif //DANGERENGINE_SLSTRANSFORM_H
