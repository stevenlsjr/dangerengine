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


/**
 * @brief convenience function for returning a
 * vec2 operation by value
 */
kmVec2 sls_vec2_add(const kmVec2 *a, const kmVec2 *b);


kmVec3 sls_vec3_add(const kmVec3 *a, const kmVec3 *b);


kmVec4 sls_vec4_add(const kmVec4 *a, const kmVec4 *b);

size_t sls_nearest_squarelu(size_t x);

static inline
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

static inline
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

static inline kmVec2 *sls_vec2_from_angle(kmVec2 *out, float rot)
{
  if (out) {
    out->x = cosf(rot);
    out->y = sinf(rot);
  }
  return out;
}


static inline double sls_to_radians(double degrees)
{
  return degrees * M_PI / 180.0;
}

static inline double sls_to_degrees(double radians)
{
  return radians * 180.0 / M_PI;
}

static inline
bool sls_vec2_near(kmVec2 const *a, kmVec2 const *b, float epsilon)
{
  assert(a && b);

  return sls_nearf(a->x, b->x, epsilon) &&
         sls_nearf(a->y, b->y, epsilon);
}


#endif //DANGERENGINE_MATHMACS_H
