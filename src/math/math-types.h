/**
 * Created by Steven on 6/23/15.
 *
 * @file math/math-types.h
 * @brief
 */

#ifndef DANGERENGINE_MATH_TYPES_H
#define DANGERENGINE_MATH_TYPES_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <string.h>
#include "../slsutils.h"

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

typedef struct slsVRect {
  kmVec2 position;
  kmVec2 size;
} slsVRect;

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
  v.kvec = (kmVec2){x, y};
  return v;
}

static inline slsVec3 sls_make_vec3(float x, float y, float z)
{

  slsVec3 v;
  v.kvec = (kmVec3){x, y, z};
  return v;
}

static inline slsVec4 sls_make_vec4(float x, float y, float z, float w)
{
  slsVec4 v;
  v.kvec = (kmVec4){x, y, z, w};
  return v;
}

static inline void sls_log_mat4(kmMat4 const *mat)
{
  sls_log_info("mat4x4 [");
  for (int j=0; j< 4; j++) {
    float const *row = mat->mat + j * 4;
    fprintf(stderr, "\t%f %f %f %f\n", row[0], row[1], row[2], row[3]);
  }
  fprintf(stderr, "]");
}

/*---------------------------------------*
 * slsIPoint operations
 *---------------------------------------*/
slsBool sls_ipoint_eq(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_add(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_sub(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_mul(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

slsIPoint sls_ipoint_idiv(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

kmVec2 sls_ipoint_to_vec2(slsIPoint const *a) SLS_NONNULL(1) SLS_PURE;

static inline kmVec2 sls_array_to_vec2(float const *arr) {
  kmVec2 v = {};
  const size_t size = 2;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.y == arr[1]);
  }

  return v;
};

static inline kmVec3 sls_array_to_vec3(float const *arr) {
  kmVec3 v = {};
  const size_t size = 3;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.z == arr[2]);
  }

  return v;
};

static inline kmVec4 sls_array_to_vec4(float const *arr) {
  kmVec4 v = {};
  const size_t size = 4;

  if (arr) {
    assert(memcpy(&v, arr, sizeof(float) * size));
    assert(v.w == arr[3]);
  }

  return v;
};

// TODO(Steven): c11 overloading functions

#if 1
#   define sls_add(res, aptr, bptr) _Generic((res)  \
      kmVec2*:  kmVec2Add((res), (aptr), (bprt)),    \
      kmVec3*:  kmVec3Add((res), (aptr), (bprt)),    \
      kmVec4*:  kmVec4Add((res), (aptr), (bprt)),    \
      slsVec2*: kmVec2Add((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec3*: kmVec3Add((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec4*: kmVec4Add((res->kvec), (aptr->kvec), (bprt->kvec)),    \
    )

#   define sls_sub(res, aptr, bptr) _Generic((res)  \
      kmVec2*:  kmVec2Sub((res), (aptr), (bprt)),    \
      kmVec3*:  kmVec3Sub((res), (aptr), (bprt)),    \
      kmVec4*:  kmVec4Sub((res), (aptr), (bprt)),    \
      slsVec2*: kmVec2Sub((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec3*: kmVec3Sub((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec4*: kmVec4Sub((res->kvec), (aptr->kvec), (bprt->kvec)),    \
    )
#   define sls_mul(res, aptr, bptr) _Generic((res)  \
      kmVec2*:  kmVec2Mul((res), (aptr), (bprt)),    \
      kmVec3*:  kmVec3Mul((res), (aptr), (bprt)),    \
      kmVec4*:  kmVec4Mul((res), (aptr), (bprt)),    \
      slsVec2*: kmVec2Mul((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec3*: kmVec3Mul((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec4*: kmVec4Mul((res->kvec), (aptr->kvec), (bprt->kvec)),    \
    )
#   define sls_div(res, aptr, bptr) _Generic((res)  \
      kmVec2*:  kmVec2Div((res), (aptr), (bprt)),    \
      kmVec3*:  kmVec3Div((res), (aptr), (bprt)),    \
      kmVec4*:  kmVec4Div((res), (aptr), (bprt)),    \
      slsVec2*: kmVec2Div((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec3*: kmVec3Div((res->kvec), (aptr->kvec), (bprt->kvec)),    \
      slsVec4*: kmVec4Div((res->kvec), (aptr->kvec), (bprt->kvec)),    \
    )

#   define sls_dot(a, b) _Generic((res)  \
      kmVec2*:  kmVec2Dot((a), (b)),    \
      kmVec3*:  kmVec3Dot((a), (b)),    \
      kmVec4*:  kmVec4Dot((a), (b)),    \
      slsVec2*: kmVec2Dot((a->kvec), (b->kvec)),    \
      slsVec3*: kmVec3Dot((a->kvec), (b->kvec))),    \
      slsVec4*: kmVec4Dot((a->kvec), (b->kvec))),    \
      const kmVec2*:  kmVec2Dot((a), (b)),    \
      const kmVec3*:  kmVec3Dot((a), (b)),    \
      const kmVec4*:  kmVec4Dot((a), (b)),    \
      const slsVec2*: kmVec2Dot((a->kvec), (b->kvec)),    \
      const slsVec3*: kmVec3Dot((a->kvec), (b->kvec))),    \
      const slsVec4*: kmVec4Dot((a->kvec), (b->kvec))),    \
    )


#endif


#endif //DANGERENGINE_MATH_TYPES_H

