//
// Created by Steven on 7/2/15.
//

#include "hashtable.h"
#include <stdlib.h>


void *sls_string_cpy(void const *str);
int sls_string_cmp(void const *lhs, void const *rhs);

void *sls_intref_cpy(void const *str);
int sls_intref_cmp(void const *lhs, void const *rhs);

static const slsCallbackTable string_calbacks = {
    .copy_fn = sls_string_cpy,
    .cmp_fn = sls_string_cmp,
    .free_fn = free
};
static const slsCallbackTable intref_callbacks = {
    .copy_fn = sls_intref_cpy,
    .cmp_fn = sls_intref_cmp,
    .free_fn = free
};

slsCallbackTable const *sls_string_callbacks() {
  return &string_calbacks;
}

slsCallbackTable const *sls_int_callbacks() {
  return &intref_callbacks;
}

void *sls_string_cpy(void const *str)
{
  if (!str) {
    sls_log_err("null arguments: str 0x%x", str);
    return NULL;
  }
  size_t max_size = 100;
  char *cpy = calloc(sizeof(char), max_size + 1);

  strlcpy(cpy, str, max_size);
  cpy[max_size] = '\0';

  return cpy;
}
int sls_string_cmp(void const *lhs, void const *rhs)
{
  if (!lhs || !rhs) {
    sls_log_err("null arguments: lhs 0x%x, rhs 0x%x", lhs, rhs);
    return -1;
  }
  size_t max_size = 100;

  return strncmp(lhs, rhs, max_size);

}

void *sls_intref_cpy(void const *iptr)
{
  if (!iptr) {
    sls_log_err("null arguments: iptr 0x%x", iptr);
    return NULL;
  }
  int *cpy = malloc(sizeof(int));

  cpy = memcpy(cpy, iptr, sizeof(int));


  return cpy;
}
int sls_intref_cmp(void const *lhs, void const *rhs)
{
  if (!lhs || !rhs) {
    sls_log_err("null arguments: lhs 0x%x, rhs 0x%x", lhs, rhs);
    return -1;
  }

  int a = *((int*)lhs);
  int b = *((int*)rhs);

  if (a == b) {
    return 0;
  } else {
    return a < b? -1 : 1;
  }

}