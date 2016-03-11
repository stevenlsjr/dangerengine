/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 2/20/16, Steven
 * 
 **/
#ifndef DANGERENGINE_OVERLOADS_H
#define DANGERENGINE_OVERLOADS_H

#include <kazmath/kazmath.h>

#ifdef __cplusplus

static inline
kmVec2 *sls_add(kmVec2 *out, kmVec2 const *lhs, kmVec2 const *rhs)
{
  return kmVec2Add(out, lhs, rhs);
}

static inline
kmVec3 *sls_add(kmVec3 *out, kmVec3 const *lhs, kmVec3 const *rhs)
{
  return kmVec3Add(out, lhs, rhs);
}

static inline
kmVec4 *sls_add(kmVec4 *out, kmVec4 const *lhs, kmVec4 const *rhs)
{
  return kmVec4Add(out, lhs, rhs);
}

// subtraction

static inline
kmVec2 *sls_sub(kmVec2 *out, kmVec2 const *lhs, kmVec2 const *rhs)
{
  return kmVec2Subtract(out, lhs, rhs);
}

static inline
kmVec3 *sls_sub(kmVec3 *out, kmVec3 const *lhs, kmVec3 const *rhs)
{
  return kmVec3Subtract(out, lhs, rhs);
}

static inline
kmVec4 *sls_sub(kmVec4 *out, kmVec4 const *lhs, kmVec4 const *rhs)
{
  return kmVec4Subtract(out, lhs, rhs);
}

// multiplication

static inline
kmVec2 *sls_mul(kmVec2 *out, kmVec2 const *lhs, kmVec2 const *rhs)
{
  return kmVec2Mul(out, lhs, rhs);
}

static inline
kmVec3 *sls_mul(kmVec3 *out, kmVec3 const *lhs, kmVec3 const *rhs)
{
  return kmVec3Mul(out, lhs, rhs);
}

static inline
kmVec4 *sls_mul(kmVec4 *out, kmVec4 const *lhs, kmVec4 const *rhs)
{
  return kmVec4Mul(out, lhs, rhs);
}


static inline
kmVec2 *sls_div(kmVec2 *out, kmVec2 const *lhs, kmVec2 const *rhs)
{
  return kmVec2Div(out, lhs, rhs);
}

static inline
kmVec3 *sls_div(kmVec3 *out, kmVec3 const *lhs, kmVec3 const *rhs)
{
  return kmVec3Div(out, lhs, rhs);
}

static inline
kmVec4 *sls_div(kmVec4 *out, kmVec4 const *lhs, kmVec4 const *rhs)
{
  return kmVec4Div(out, lhs, rhs);
}




#else

// c11 generic macros for implementing polymorphic math funcions

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
      slsVec4*: kmVec4Dot((a->kvec), (b->kvec)))    \
    )



#endif

#endif //DANGERENGINE_OVERLOADS_H
