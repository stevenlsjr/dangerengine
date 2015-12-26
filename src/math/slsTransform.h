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
#include <kazmath/vec4.h>
#include <slsutils.h>
#include "slsTransform_private.h"

typedef struct slsTransform slsTransform;

typedef struct slsTransform_p slsTransform_p;


/**
 * @brief Abstract type representing a generic modelview matrix.
 * Does not keep track of rotation, position or scale, rather manages
 * modelview and coresponding normal matricies.
 */
struct slsTransform {
  slsTransform_p priv;
};

slsTransform *sls_transform_init(slsTransform *self) SLS_NONNULL(1);
slsTransform *sls_transform_dtor(slsTransform *self) SLS_NONNULL(1);

kmMat4 const *sls_transform_get_modelview(slsTransform *self) SLS_NONNULL(1);
kmMat4 const *sls_transform_get_inverse_modelview(slsTransform *self);
kmMat4 const *sls_transform_get_normalview(slsTransform *self);

kmMat4 const *sls_transform_set_modelview(slsTransform *self, kmMat4 const *mv) SLS_NONNULL(1, 2);

kmQuaternion *sls_transform_rotation(slsTransform const *self, kmQuaternion *out);
kmVec4 sls_transform_position(slsTransform const *self);
kmVec4 sls_transform_scale(slsTransform const *self, kmQuaternion *out);



#endif //DANGERENGINE_SLSTRANSFORM_H
