//
// Created by Steven on 4/27/15.
//

#ifndef DANGERENGINE_MATHMACS_H
#define DANGERENGINE_MATHMACS_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <math.h>
#include <stddef.h>

kmVec2 sls_vec2_add(const kmVec2 *a, const kmVec2 *b);


kmVec3 sls_vec3_add(const kmVec3 *a, const kmVec3 *b);


kmVec4 sls_vec4_add(const kmVec4 *a, const kmVec4 *b);

size_t sls_nearest_squarelu(size_t x);


#ifndef __cplusplus

#define sls_add(out, a, b) _Generic((out),    \
    kmVec2 *: kmVec2Add(out, a, b), \
    kmVec3 *: kmVec3Add(out, a, b), \
    kmVec4 *: kmVec4Add(out, a, b))

#define sls_sub(out, a, b) _Generic((out),    \
    kmVec2 *: kmVec2Subtract(out, a, b), \
    kmVec3 *: kmVec3Subtract(out, a, b), \
    kmVec4 *: kmVec4Subtract(out, a, b))

#define sls_mul(out, a, b) _Generic((out),    \
    kmVec2 *: kmVec2Mul(out, a, b), \
    kmVec3 *: kmVec3Mul(out, a, b), \
    kmVec4 *: kmVec4Mul(out, a, b),  \
    kmMat3 *: kmMat3Multiply(out, a, b) \
    kmMat4 *: kmMat4Multiply(out, a, b))

#define sls_div(out, a, b) _Generic((out),    \
    kmVec2 *: kmVec2Div(out, a, b), \
    kmVec3 *: kmVec3Div(out, a, b), \
    kmVec4 *: kmVec4Div(out, a, b))

#endif

#endif //DANGERENGINE_MATHMACS_H
