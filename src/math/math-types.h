/**
 * Created by Steven on 6/23/15.
 *
 * @file math/math-types.h
 * @brief
 */

#ifndef DANGERENGINE_MATH_TYPES_H
#define DANGERENGINE_MATH_TYPES_H

#include <kazmath/vec2.h>
#include "../slsutils.h"

/**
 * @brief simple structure for a integer point
 */
typedef struct slsIPoint {
  int x, y;
} slsIPoint;

/*---------------------------------------*
 * slsIPoint operations
 *---------------------------------------*/
slsBool  sls_ipoint_eq(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;
slsIPoint sls_ipoint_add(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;
slsIPoint sls_ipoint_sub(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;
slsIPoint sls_ipoint_mul(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;
slsIPoint sls_ipoint_idiv(slsIPoint const *a, slsIPoint const *b) SLS_NONNULL(1, 2) SLS_PURE;

kmVec2 sls_ipoint_to_vec2(slsIPoint const *a) SLS_NONNULL(1) SLS_PURE;


#endif //DANGERENGINE_MATH_TYPES_H

