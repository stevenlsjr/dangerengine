/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/18/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSTRANSFORM3D_H
#define DANGERENGINE_SLSTRANSFORM3D_H

#include "slsTransform.h"
#include <slsutils.h>
#include <kazmath/vec4.h>

typedef struct slsTransform3D_p slsTransform3D_p;
typedef struct slsTransform3D slsTransform3D;

struct slsTransform3D {
  slsTransform transform;
  slsTransform3D_p *priv;
};

slsTransform3D *sls_transform3d_init(slsTransform3D *self) SLS_NONNULL(1);

slsTransform3D *sls_transform3d_dtor(slsTransform3D *self) SLS_NONNULL(1);

void sls_transform3d_bind(slsTransform3D *self) SLS_NONNULL(1);

bool sls_transform3d_is_bound(slsTransform3D *self) SLS_NONNULL(1);

kmVec3 const * sls_transform3d_position(slsTransform3D *self) SLS_NONNULL(1);

kmVec3 const *sls_transform3d_scale(slsTransform3D *self) SLS_NONNULL(1);

kmQuaternion const *sls_transform3d_rotation(slsTransform3D *self) SLS_NONNULL(1);


slsTransform3D *sls_transform3d_set_position(slsTransform3D *self,
                                             kmVec3 const *in) SLS_NONNULL(1, 2);

slsTransform3D *sls_transform3d_set_scale(slsTransform3D *self,
                                          kmVec3 const *in) SLS_NONNULL(1, 2);

slsTransform3D *sls_transform3d_set_rotation(slsTransform3D *self,
                                             kmQuaternion const *in) SLS_NONNULL(1, 2);

#endif //DANGERENGINE_SLSTRANSFORM3D_H
