/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 3/28/16, Steven
 * 
 **/

#include "slsSimd.h"
#include <assert.h>
#include <math.h>
#include <smmintrin.h>
#include <stdalign.h>
#include <string.h>


static_assert(__alignas_is_defined, "requires C11 _Alignas");


typedef struct vec_loader {
  alignas(16) float vec[4];
} vec_loader;



sls4Vec4Simd *sls_simdvec_from_vec4s(sls4Vec4Simd *out, slsVec4Array const *in)
{
  vec_loader loader;
  loader = (vec_loader){{in->arr[0].x, in->arr[1].x, in->arr[2].x, in->arr[3].x}};
  out->xv = _mm_load_ps(loader.vec);

  loader = (vec_loader){{in->arr[0].y, in->arr[1].y, in->arr[2].y, in->arr[3].y}};
  out->yv = _mm_load_ps(loader.vec);

  loader = (vec_loader){{in->arr[0].z, in->arr[1].z, in->arr[2].z, in->arr[3].z}};
  out->zv = _mm_load_ps(loader.vec);

  loader = (vec_loader){{in->arr[0].w, in->arr[1].w, in->arr[2].w, in->arr[3].w}};
  out->wv = _mm_load_ps(loader.vec);
  return out;
}

