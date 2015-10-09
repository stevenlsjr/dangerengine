/**
 * Created by Steven on 4/27/15.
 *
 * @file math/math-types.h
 * @brief
 */



#ifndef DANGERENGINE_MATHMACS_H
#define DANGERENGINE_MATHMACS_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>


#define sls_near(a, b, e) _Generic(a,   \
    double: sls_neard(a, b, e),         \
    float: sls_nearf(a, b, e))


/**
 * @brief convenience function for returning a
 * vec2 operation by value
 */
kmVec2 sls_vec2_add(const kmVec2 *a, const kmVec2 *b);


kmVec3 sls_vec3_add(const kmVec3 *a, const kmVec3 *b);


kmVec4 sls_vec4_add(const kmVec4 *a, const kmVec4 *b);

size_t sls_nearest_squarelu(size_t x);


static inline bool sls_neard(double a, double b, double epsilon)
{
  return fabs(a - b) < epsilon;
}

static inline bool sls_nearf(float a, float b, float epsilon)
{
  return fabs(a - b) < epsilon;
}


#endif //DANGERENGINE_MATHMACS_H
