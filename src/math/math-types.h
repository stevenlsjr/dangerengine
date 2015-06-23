/**
 * Created by Steven on 6/23/15.
 *
 * @file math/math-types.h
 * @brief
 */

#ifndef DANGERENGINE_MATH_TYPES_H
#define DANGERENGINE_MATH_TYPES_H

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
slsBool sls_ipoint_eq(slsIPoint const *a, slsIPoint const *b);
slsIPoint sls_ipoint_add(slsIPoint const *a, slsIPoint const *b);
slsIPoint sls_ipoint_sub(slsIPoint const *a, slsIPoint const *b);
slsIPoint sls_ipoint_mul(slsIPoint const *a, slsIPoint const *b);
slsIPoint sls_ipoint_idiv(slsIPoint const *a, slsIPoint const *b);


#endif //DANGERENGINE_MATH_TYPES_H

