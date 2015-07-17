//
// Created by Steven on 7/17/15.
//

#include <string.h>

#include <stdlib.h>

#include "../slsutils.h"


static const size_t const max_string_len = (size_t)500e6; // 500 megabytes!


void *sls_copy_assign(void const *ptr)
{
  return (void*)ptr;
}

void *sls_copy_string(void const *str)
{




  char const *typed_str = str;


  return NULL;
}

int sls_cmp_string(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }




  return 0;
}

int sls_cmp_intptr(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }
  return 0;
}

int sls_cmp_uintptr(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }

  unsigned int ai = *(int unsigned const *)a;
  unsigned int bi = *(int unsigned const *)b;

  if (ai == bi) {
    return 0;
  } else {
    return ai > bi? -1: 1;
  }

}

