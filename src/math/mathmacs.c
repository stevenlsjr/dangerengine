//
// Created by Steven on 5/2/15.
//

#include "mathmacs.h"

kmVec2 sls_vec2_add(const kmVec2 *a, const kmVec2 *b)
{
  kmVec2 res;
  kmVec2Add(&res, a, b);
  return res;
}

kmVec3 sls_vec3_add(const kmVec3 *a, const kmVec3 *b)
{
  kmVec3 res;

  kmVec3Add(&res, a, b);

  return res;
}

kmVec4 sls_vec4_add(const kmVec4 *a, const kmVec4 *b)
{
  kmVec4 res;

  kmVec4Add(&res, a, b);



  return res;
}

size_t sls_nearest_squarelu(size_t x)
{
  double exponent = log(x) / log(2.0);
  return (size_t) pow(2, ceil(exponent));
}
