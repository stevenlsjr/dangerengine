/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/13/15, Steven
 * 
 **/
#ifndef DANGERENGINE_CPPAPI_H
#define DANGERENGINE_CPPAPI_H


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
  return a > b?
         a: b;
}

template <typename T_NUM>
static inline float SLS_MIN(T_NUM const &a, T_NUM const &b)
{
  return a < b?
         a: b;
}

#else

#define sls_near(a, b, e) _Generic(a,   \
    double: sls_neard(a, b, e),         \
    float: sls_nearf(a, b, e))


#define SLS_MIN(a, b) ({ \
  typeof(a) __a = (a); \
  typeof(b) __b = (b); \
  __a < __b? __a: __b; \
})

#define SLS_MAX(a, b) ({ \
  typeof(a) __a = (a); \
  typeof(b) __b = (b); \
  __a > __b? __a: __b; \
})

#endif



#endif //DANGERENGINE_CPPAPI_H
