//
// Created by Steven on 5/2/15.
//

#include "slsMathUtils.h"

size_t sls_nearest_squarelu(size_t x)
{
  double exponent = log(x) / log(2.0);
  return (size_t)pow(2, ceil(exponent));
}

bool sls_neard(double a, double b, double epsilon)
{
  const double abs_a = fabs(a);
  const double abs_b = fabs(b);
  const double diff = fabs(a - b);

  if (a == b) { // shortcut
    return true;
  } else if (a * b == 0) { // a or b or both are zero
    // relative error is not meaningful here
    return diff < (epsilon * epsilon);
  } else { // use relative error
    return diff / (abs_a + abs_b) < epsilon;
  }
}

bool sls_nearf(float a, float b, float epsilon)
{

  const float abs_a = fabsf(a);
  const float abs_b = fabsf(b);
  const float diff = fabsf(a - b);

  if (a == b) { // shortcut
    return true;
  } else if (a * b == 0) { // a or b or both are zero
    // relative error is not meaningful here
    return diff < (epsilon * epsilon);
  } else { // use relative error
    return diff / (abs_a + abs_b) < epsilon;
  }
}

kmVec2 *sls_vec2_from_angle(kmVec2 *out, float rot)
{
  if (out) {
    out->x = cosf(rot);
    out->y = sinf(rot);
  }
  return out;
}

double sls_to_degrees(double radians) { return radians * 180.0 / M_PI; }

double sls_to_radians(double degrees) { return degrees * M_PI / 180.0; }

bool sls_vec2_near(kmVec2 const *a, kmVec2 const *b, float epsilon)
{
  assert(a && b);

  return sls_nearf(a->x, b->x, epsilon) && sls_nearf(a->y, b->y, epsilon);
}

kmVec2 sls_vec2p_add(kmVec2 const *rhs, kmVec2 const *lhs)
{
  kmVec2 res;

  kmVec2Add(&res, rhs, lhs);

  return res;
}

kmVec2 sls_vec2p_sub(kmVec2 const *rhs, kmVec2 const *lhs)
{
  kmVec2 res;
  kmVec2Subtract(&res, rhs, lhs);

  return res;
}

kmVec2 sls_vec2p_mul(kmVec2 const *rhs, kmVec2 const *lhs)
{
  kmVec2 res;
  kmVec2Mul(&res, rhs, lhs);

  return res;
}

kmVec2 sls_vec2p_div(kmVec2 const *rhs, kmVec2 const *lhs)
{
  kmVec2 res;
  kmVec2Div(&res, rhs, lhs);

  return res;
}

kmVec3 sls_vec3p_add(kmVec3 const *rhs, kmVec3 const *lhs)
{
  kmVec3 res;
  kmVec3Add(&res, rhs, lhs);
  return res;
}

kmVec3 sls_vec3p_sub(kmVec3 const *rhs, kmVec3 const *lhs)
{
  kmVec3 res;
  kmVec3Subtract(&res, rhs, lhs);

  return res;
}

kmVec3 sls_vec3p_mul(kmVec3 const *rhs, kmVec3 const *lhs)
{
  kmVec3 res;
  kmVec3Mul(&res, rhs, lhs);

  return res;
}

kmVec3 sls_vec3p_div(kmVec3 const *rhs, kmVec3 const *lhs)
{
  kmVec3 res;
  kmVec3Div(&res, rhs, lhs);

  return res;
}

kmVec4 sls_vec4p_add(kmVec4 const *rhs, kmVec4 const *lhs)
{
  kmVec4 res;
  kmVec4Add(&res, rhs, lhs);

  return res;
}

kmVec4 sls_vec4p_sub(kmVec4 const *rhs, kmVec4 const *lhs)
{
  kmVec4 res;
  kmVec4Subtract(&res, rhs, lhs);

  return res;
}

kmVec4 sls_vec4p_mul(kmVec4 const *rhs, kmVec4 const *lhs)
{
  kmVec4 res;
  kmVec4Mul(&res, rhs, lhs);

  return res;
}

kmVec4 sls_vec4p_div(kmVec4 const *rhs, kmVec4 const *lhs)
{
  kmVec4 res;
  kmVec4Div(&res, rhs, lhs);

  return res;
}
