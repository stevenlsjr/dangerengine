//
// Created by Steven on 4/27/15.
//

#ifndef DANGERENGINE_MATHMACS_H
#define DANGERENGINE_MATHMACS_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>

kmVec2 sls_vec2_add(const kmVec2 *a, const kmVec2 *b)
{
    kmVec2 res;
    kmVec2Add(&res, a, b);
    return res;
}

kmVec3 sls_vec3_add(const kmVec3 *a, const kmVec3 *b)
{
    kmVec3 res;

    kmVec3Add(&res, a, b);

    return res;
}



kmVec4 sls_vec4_add(const kmVec4 *a, const kmVec4 *b)
{
    kmVec4 res;

    kmVec4Add(&res, a, b);

    

    return res;
}



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



#endif //DANGERENGINE_MATHMACS_H
