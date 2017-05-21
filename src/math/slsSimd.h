/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 3/28/16, Steven
 *
 **/
#ifndef DANGERENGINE_SLSSIMD_H
#define DANGERENGINE_SLSSIMD_H

#include <kazmath/vec4.h>
#include <slsmacros.h>
#include <smmintrin.h>
#include <stddef.h>

#define SLS_SHUFFLE_MASK(z, y, x, w) (z << 6) | (y << 4) | (x << 2) | w

/**
 * @brief a struct of 4 _m128 values
 * representing the x, y, z, and w components of 4 vec4s
 */
typedef struct sls4Vec4Simd {
  __m128 xv;
  __m128 yv;
  __m128 zv;
  __m128 wv;
} sls4Vec4Simd;

typedef struct slsVec4Array {
  kmVec4 arr[4];
} slsVec4Array;

/**
 * @brief converts an array of kmVec4s to a struct-of-array
 * representation useful for vectorization.
 *
 * @param n. Number of values to store. If n is greater than 4, it will
 * coerce to 4 (sls4Vec4Simd only stores 4 vectors).
 * @param identity. If n is less than 4, this will be inserted to the rest of
 *the values
 */
sls4Vec4Simd* sls_simdvec_from_vec4s(sls4Vec4Simd* out, slsVec4Array const* in)
  SLS_NONNULL(1, 2);

static inline sls4Vec4Simd sls_simdvec_from_vec4s_val(slsVec4Array const* in)
{
  sls4Vec4Simd res;
  sls_simdvec_from_vec4s(&res, in);
  return res;
}

#endif // DANGERENGINE_SLSSIMD_H
