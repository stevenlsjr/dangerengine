/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/13/15, Steven
 *
 **/
#ifndef DANGERENGINE_CPPAPI_H
#define DANGERENGINE_CPPAPI_H

#include <math.h>

#if defined(__cplusplus)

static inline float sls_near(float a, float b)
{
  extern float sls_nearf(float, float);
  return sls_nearf(a, b);
}

static inline double sls_near(double a, double b)
{
  extern double sls_neard(double, double);
  return sls_neard(a, b);
}

template <typename T_NUM>
static inline float SLS_MAX(T_NUM const &a, T_NUM const &b)
{
  return a > b ? a : b;
}

template <typename T_NUM>
static inline float SLS_MIN(T_NUM const &a, T_NUM const &b)
{
  return a < b ? a : b;
}

#else

#define sls_near(a, b, e)                                                      \
  _Generic(a, double : sls_neard(a, b, e), float : sls_nearf(a, b, e))

/**
 * @brief Pseudo-generic min macro
 * @detail NOTE: this macro declares variables. It may not be
 * hygenic. However, it does not paste the input parameters more than once,
 * making it safe to use parameters returned from a function. Uses a GCC
 * extension
 */
#define SLS_MIN_fallback(a, b)                                                 \
  ({                                                                           \
    typeof(a) __a = (a);                                                       \
    typeof(b) __b = (b);                                                       \
    __a < __b ? __a : __b;                                                     \
  })

#define SLS_MIN(a, b)                                                          \
  _Generic(a, float                                                            \
           : fminf(a, b), double                                               \
           : fmin(a, b), long double                                           \
           : fminl(a, b), default                                              \
           : SLS_MIN_fallback(a, b))

/**
 * @brief Pseudo-generic max macro
 * @detail NOTE: this macro declares variables. It may not be
 * hygenic. However, it does not paste the input parameters more than once,
 * making it safe to use parameters returned from a function. Uses a GCC
 * extension
 */
#define SLS_MAX_fallback(a, b)                                                 \
  ({                                                                           \
    typeof(a) __a = (a);                                                       \
    typeof(b) __b = (b);                                                       \
    __a > __b ? __a : __b;                                                     \
  })

#define SLS_MAX(a, b)                                                          \
  _Generic(a, float                                                            \
           : fmaxf(a, b), double                                               \
           : fmax(a, b), long double                                           \
           : fmaxl(a, b), default                                              \
           : SLS_MAX_fallback(a, b))

#endif

#endif // DANGERENGINE_CPPAPI_H
