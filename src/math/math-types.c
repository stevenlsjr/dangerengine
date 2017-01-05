//
// Created by Steven on 6/23/15.
//

#include "math-types.h"

bool sls_ipoint_eq(slsIPoint const *a, slsIPoint const *b)
{
  return a->x == b->x && a->y == b->y;
}

slsIPoint sls_ipoint_add(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){ a->x + b->x, a->y + b->y };
}

slsIPoint sls_ipoint_sub(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){ a->x - b->x, a->y - b->y };
}

slsIPoint sls_ipoint_mul(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){ a->x * b->x, a->y * b->y };
}

slsIPoint sls_ipoint_idiv(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){ a->x / b->x, a->y / b->y };
}

kmVec2 sls_ipoint_to_vec2(slsIPoint const *a)
{
  kmVec2 result = { (float)a->x, (float)a->y };

  return result;
}

kmMat4 *sls_mat4_normalmat(kmMat4 *out, kmMat4 const *modelview)
{
  kmMat4 tmp;

  kmMat4Inverse(&tmp, modelview);
  kmMat4Transpose(out, &tmp);

  return out;
}
