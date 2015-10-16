//
// Created by Steven on 8/16/15.
//

#ifndef DANGERENGINE_SLSTRANSFORM2D_H
#define DANGERENGINE_SLSTRANSFORM2D_H

#include "../slsutils.h"
#include <kazmath/kazmath.h>

typedef  struct slsEntity slsEntity;


typedef struct slsTransform2D slsTransform2D;
typedef  struct slsKinematic2D slsKinematic2D;


struct slsTransform2D {
  kmVec2 pos;
  kmVec2 scale;
  float rot;

  int z_layer;

  kmMat4 model_view;

  slsEntity *entity;
};

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

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out, slsTransform2D const *in) SLS_NONNULL(1, 2);

slsBool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b) SLS_NONNULL(1, 2);
slsBool sls_transform2D_near(slsTransform2D const *a,
                             slsTransform2D const *b,
                             float epsilon) SLS_NONNULL(1, 2);

slsTransform2D *sls_transform2d_init(slsTransform2D *self) SLS_NONNULL(1);

kmVec2 sls_transform2d_local_to_world(slsTransform2D const *self,
                                      kmVec2 const *in) SLS_NONNULL(1, 2);
kmVec2 sls_transform2d_world_to_local(slsTransform2D const *self,
                                      kmVec2 const *in) SLS_NONNULL(1, 2);


#endif //DANGERENGINE_SLSTRANSFORM2D_H
