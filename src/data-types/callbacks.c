//
// Created by Steven on 7/17/15.
//

#include <string.h>

#include <stdlib.h>

#include "../slsutils.h"


static const size_t max_string_len = (size_t) 500e6; // 500 megabytes


void *sls_copy_assign(void const *ptr)
{
  return (void*)ptr;
}

void *sls_copy_string(void const *str)
{
  char const *typed_str = str;
  char *cpy = NULL;
  size_t size = strnlen(typed_str, max_string_len);
  cpy = calloc(size+1, sizeof(char));
  sls_checkmem(cpy);

  strlcpy(cpy, typed_str, size);

  return cpy;
error:
  if (cpy) {free(cpy);}
  return NULL;
}

int sls_cmp_string(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }

  return strncmp(a, b, max_string_len);
}

int sls_cmp_intptr(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }
 int ai = *(int const *)a;
 int bi = *(int const *)b;

  return bi - ai;
}

int sls_cmp_uintptr(void const *a, void const *b)
{
  if (!a || !b) {
    sls_log_err("invalid null arguments! %p %p\n", a, b);
    return 0;
  }

  unsigned int ai = *(int unsigned const *)a;
  unsigned int bi = *(int unsigned const *)b;

  return (int)bi - (int)ai;
}

