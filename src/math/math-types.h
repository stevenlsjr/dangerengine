/**
 * Created by Steven on 6/23/15.
 *
 * @file math/math-types.h
 * @brief
 */

#ifndef DANGERENGINE_MATH_TYPES_H
#define DANGERENGINE_MATH_TYPES_H

#include "../slsutils.h"
#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <string.h>

/**
 * @brief simple structure for a integer point
 */
typedef struct slsIPoint {
  int x, y;
} slsIPoint;

typedef union slsIRect {
  struct {
    slsIPoint position;
    slsIPoint size;
  };
  struct {
    int x;
    int y;
    int width;
    int height;
  };
} slsIRect;

typedef struct slsFRect {
  kmVec2 position;
  kmVec2 size;
} slsFRect;

typedef union slsVec2 {
  struct {
    float x;
    float y;
  };
  struct {
    float u;
    float v;
  };
  struct {
    float r;
    float g;
  };
  struct kmVec2 kvec;
} slsVec2;

typedef union slsVec3 {
  struct {
    float x;
    float y;
    float z;
  };
  struct {
    float u;
    float v;
    float s;
  };
  struct {
    float r;
    float g;
    float b;
  };
  slsVec2 xy;
  kmVec3 kvec;
} slsVec3;

typedef union slsVec4 {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
  struct {
    float u;
    float v;
    float s;
    float t;
  };

  struct {
    float r;
    float g;
    float b;
    float a;
  };

  slsVec2 xy;
  slsVec3 xyz;

  kmVec4 kvec;
} slsVec4;

static inline slsVec2 sls_make_vec2(float x, float y)
{
  slsVec2 v;
  v.kvec = (kmVec2){ x, y };
  return v;
}

static inline slsVec3 sls_make_vec3(float x, float y, float z)
{

  slsVec3 v;
  v.kvec = (kmVec3){ x, y, z };
  return v;
}

static inline slsVec4 sls_make_vec4(float x, float y, float z, float w)
{
  slsVec4 v;
  v.kvec = (kmVec4){ x, y, z, w };
  return v;
}

static inline kmMat3 sls_mat3_id()
{
  kmMat3 mat;
  kmMat3Identity(&mat);
  return mat;
}

static inline kmMat4 sls_mat4_id()
{
  kmMat4 mat;
  kmMat4Identity(&mat);
  return mat;
}

static inline kmQuaternion sls_quat_id()
{
  kmQuaternion q;
  kmQuaternionIdentity(&q);
  return q;
}

static inline void sls_log_mat4(kmMat4 const* mat)
{
  sls_log_info("mat4x4 [");
  for (int j = 0; j < 4; j++) {
    float const* row = mat->mat + j * 4;
    fprintf(stderr, "\t%f %f %f %f\n", row[0], row[1], row[2], row[3]);
  }
  fprintf(stderr, "]");
}

/*---------------------------------------*
 * slsIPoint operations
 *---------------------------------------*/
bool sls_ipoint_eq(slsIPoint const* a, slsIPoint const* b)
  SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_add(slsIPoint const* a, slsIPoint const* b)
  SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_sub(slsIPoint const* a, slsIPoint const* b)
  SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_mul(slsIPoint const* a, slsIPoint const* b)
  SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_idiv(slsIPoint const* a, slsIPoint const* b)
  SLS_NONNULL(1, 2) SLS_PURE;

kmVec2 sls_ipoint_to_vec2(slsIPoint const* a) SLS_NONNULL(1) SLS_PURE;

static inline kmVec2 sls_array_to_vec2(float const* arr)
{
  kmVec2 v = {};
  const size_t size = 2;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.y == arr[1]);
  }

  return v;
};

static inline kmVec3 sls_array_to_vec3(float const* arr)
{
  kmVec3 v = {};
  const size_t size = 3;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.z == arr[2]);
  }

  return v;
};

static inline kmVec4 sls_array_to_vec4(float const* arr)
{
  kmVec4 v = {};
  const size_t size = 4;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.w == arr[3]);
  }

  return v;
};

kmMat4* sls_mat4_normalmat(kmMat4* out, kmMat4 const* modelview);
kmMat4* sls_mat4_invnormalmat(kmMat4* out, kmMat4 const* modelview);

#endif // DANGERENGINE_MATH_TYPES_H
