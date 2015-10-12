//
// Created by Steven on 8/16/15.
//

#ifndef DANGERENGINE_SLSTRANSFORM2D_H
#define DANGERENGINE_SLSTRANSFORM2D_H

#include "../slsutils.h"
#include <kazmath/kazmath.h>

typedef struct slsTransform2D slsTransform2D;
struct slsTransform2D {
  kmVec2 pos;
  kmVec2 scale;
  float rot;
};

struct slsKinematic2D {
  float mass;
  kmVec2 velocity;


  float rotational_inertia;
  float rotational_speed;
};

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out, slsTransform2D const *in);

slsBool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b);
slsBool sls_transform2D_near(slsTransform2D const *a,
                             slsTransform2D const *b,
                             float epsilon);



#endif //DANGERENGINE_SLSTRANSFORM2D_H
