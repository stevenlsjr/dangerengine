//
// Created by Steven on 8/16/15.
//

#ifndef DANGERENGINE_SLSTRANSFORM2D_H
#define DANGERENGINE_SLSTRANSFORM2D_H

#include "../slsutils.h"

typedef struct slsTransform2D slsTransform2D;
struct slsTransform2D {
  kmVec2 pos;
  kmVec2 scale;
  float rot;
};

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out, slsTransform2D const *in);

slsBool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b);

#endif //DANGERENGINE_SLSTRANSFORM2D_H
