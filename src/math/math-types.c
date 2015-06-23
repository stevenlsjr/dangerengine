//
// Created by Steven on 6/23/15.
//

#include "math-types.h"

slsBool sls_ipoint_eq(slsIPoint const *a,
                      slsIPoint const *b)
{
  return a->x == b->x && a->y == b->y;
}

slsIPoint sls_ipoint_add(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){
    a->x + b->x,
    a->y + b->y
  };
}

slsIPoint sls_ipoint_sub(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){
    a->x - b->x,
    a->y - b->y
  };
}

slsIPoint sls_ipoint_mul(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){
    a->x * b->x,
    a->y * b->y
  };
}

slsIPoint sls_ipoint_idiv(slsIPoint const *a, slsIPoint const *b)
{
  return (slsIPoint){
    a->x / b->x,
    a->y / b->y
  };
}
