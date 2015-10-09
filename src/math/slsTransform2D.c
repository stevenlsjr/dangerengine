//
// Created by Steven on 8/16/15.
//

#include <kazmath/mat4.h>
#include <kazmath/vec2.h>
#include <stdlib.h>
#include "slsTransform2D.h"

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out,
                                  slsTransform2D const
                                  *in)
{
  if (!out || !in) { return NULL; }

  kmMat4 m, tmp;
  kmMat4RotationZ(out, in->rot);

  tmp = *out;
  kmMat4Multiply(out, &tmp, kmMat4Scaling(&m, in->scale.x, in->scale.y, 1.0f));

  tmp = *out;
  kmMat4Multiply(out, &tmp, kmMat4Translation(&m, in->pos.x, in->pos.y, 0.0f));
  return out;
}

slsBool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b)
{
  sls_check(a, "a is NULL");
  sls_check(b, "b is NULL");

  return kmVec2AreEqual(&a->pos, &b->pos) &&
         kmVec2AreEqual(&a->scale, &b->scale) &&
         a->rot == b->rot;

  error:
  return SLS_FALSE;
}

slsBool sls_transform2D_near(slsTransform2D const *a, slsTransform2D const *b, float epsilon)
{
  
  return SLS_TRUE;
}
