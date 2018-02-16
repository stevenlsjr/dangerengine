/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 2/16/18, Steven
 * 
 **/
#ifndef DANGERENGINE_MATH_OPERATIONS_H
#define DANGERENGINE_MATH_OPERATIONS_H

#include "math-types.h"

/*
 * Scalar vector operations
 */

static inline slsVec2 *
sls_vec2_add(slsVec2 *out, slsVec2 const *rhs, slsVec2 const *lhs)
{
  out->x = rhs->x + lhs->x;
  out->y = rhs->y + lhs->y;
  return out;
}

static inline slsVec2 *
sls_vec2_sub(slsVec2 *out, slsVec2 const *rhs, slsVec2 const *lhs)
{
  out->x = rhs->x - lhs->x;
  out->y = rhs->y - lhs->y;
  return out;
}

static inline slsVec2 *
sls_vec2_mul(slsVec2 *out, slsVec2 const *rhs, slsVec2 const *lhs)
{
  out->x = rhs->x * lhs->x;
  out->y = rhs->y * lhs->y;
  return out;
}

static inline slsVec2 *
sls_vec2_div(slsVec2 *out, slsVec2 const *rhs, slsVec2 const *lhs)
{
  out->x = rhs->x / lhs->x;
  out->y = rhs->y / lhs->y;
  return out;
}

static inline slsVec3 *
sls_vec3_add(slsVec3 *out, slsVec3 const *rhs, slsVec3 const *lhs)
{
  out->x = rhs->x + lhs->x;
  out->y = rhs->y + lhs->y;
  out->z = rhs->z + lhs->z;

  return out;
}

static inline slsVec3 *
sls_vec3_sub(slsVec3 *out, slsVec3 const *rhs, slsVec3 const *lhs)
{
  out->x = rhs->x - lhs->x;
  out->y = rhs->y - lhs->y;
  out->z = rhs->z - lhs->z;

  return out;
}

static inline slsVec3 *
sls_vec3_mul(slsVec3 *out, slsVec3 const *rhs, slsVec3 const *lhs)
{
  out->x = rhs->x * lhs->x;
  out->y = rhs->y * lhs->y;
  out->z = rhs->z * lhs->z;

  return out;
}

static inline slsVec3 *
sls_vec3_div(slsVec3 *out, slsVec3 const *rhs, slsVec3 const *lhs)
{
  out->x = rhs->x / lhs->x;
  out->y = rhs->y / lhs->y;
  out->z = rhs->z / lhs->z;

  return out;
}

static inline slsVec4 *
sls_vec4_add(slsVec4 *out, slsVec4 const *rhs, slsVec4 const *lhs)
{
  out->x = rhs->x + lhs->x;
  out->y = rhs->y + lhs->y;
  out->z = rhs->z + lhs->z;
  out->w = rhs->w + lhs->w;

  return out;
}

static inline slsVec4 *
sls_vec4_sub(slsVec4 *out, slsVec4 const *rhs, slsVec4 const *lhs)
{
  out->x = rhs->x - lhs->x;
  out->y = rhs->y - lhs->y;
  out->z = rhs->z - lhs->z;
  out->w = rhs->w - lhs->w;

  return out;
}

static inline slsVec4 *
sls_vec4_mul(slsVec4 *out, slsVec4 const *rhs, slsVec4 const *lhs)
{
  out->x = rhs->x * lhs->x;
  out->y = rhs->y * lhs->y;
  out->z = rhs->z * lhs->z;
  out->w = rhs->w * lhs->w;

  return out;
}

static inline slsVec4 *
sls_vec4_div(slsVec4 *out, slsVec4 const *rhs, slsVec4 const *lhs)
{
  out->x = rhs->x / lhs->x;
  out->y = rhs->y / lhs->y;
  out->z = rhs->z / lhs->z;
  out->w = rhs->w / lhs->w;

  return out;
}

#endif //DANGERENGINE_MATH_OPERATIONS_H
