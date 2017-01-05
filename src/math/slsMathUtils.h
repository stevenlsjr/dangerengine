/**
 * Created by Steven on 4/27/15.
 *
 * @file math/math-types.h
 * @brief
 */

#ifndef DANGERENGINE_MATHMACS_H
#define DANGERENGINE_MATHMACS_H

#define _USE_MATH_DEFINES // for C
#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

size_t sls_nearest_squarelu(size_t x);

/**
 * Returns true if given double values are equal to or
 * nearly equal to by given relative error
 * @param a
 * @param b
 * @param epsilon relative error factor
 * @return true if equal or nearly equal, otherwise false
 */
bool sls_neard(double a, double b, double epsilon);

/**
 * Returns true if given float values are equal to or
 * nearly equal to by given relative error
 * @param a
 * @param b
 * @param epsilon relative error factor
 * @return true if equal or nearly equal, otherwise false
 */
bool sls_nearf(float a, float b, float epsilon);

kmVec2 *sls_vec2_from_angle(kmVec2 *out, float rot);

double sls_to_radians(double degrees);

double sls_to_degrees(double radians);

bool sls_vec2_near(kmVec2 const *a, kmVec2 const *b, float epsilon);

/*
 * by-value vector functions
 */

kmVec2 sls_vec2p_add(kmVec2 const *rhs, kmVec2 const *lhs);
kmVec2 sls_vec2p_sub(kmVec2 const *rhs, kmVec2 const *lhs);
kmVec2 sls_vec2p_mul(kmVec2 const *rhs, kmVec2 const *lhs);
kmVec2 sls_vec2p_div(kmVec2 const *rhs, kmVec2 const *lhs);

kmVec3 sls_vec3p_add(kmVec3 const *rhs, kmVec3 const *lhs);
kmVec3 sls_vec3p_sub(kmVec3 const *rhs, kmVec3 const *lhs);
kmVec3 sls_vec3p_mul(kmVec3 const *rhs, kmVec3 const *lhs);
kmVec3 sls_vec3p_div(kmVec3 const *rhs, kmVec3 const *lhs);

kmVec4 sls_vec4p_add(kmVec4 const *rhs, kmVec4 const *lhs);
kmVec4 sls_vec4p_sub(kmVec4 const *rhs, kmVec4 const *lhs);
kmVec4 sls_vec4p_mul(kmVec4 const *rhs, kmVec4 const *lhs);
kmVec4 sls_vec4p_div(kmVec4 const *rhs, kmVec4 const *lhs);

#ifndef __cplusplus

#endif //!__cplusplus

//--------------------------utilities for calling km functions by
//value----------------------------

static inline kmMat4 sls_mat4_identity_val()
{
  kmMat4 out;
  kmMat4Identity(&out);
  return out;
}

#endif // DANGERENGINE_MATHMACS_H
