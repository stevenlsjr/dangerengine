//
// Created by Steven on 4/25/15.
//

#include "slsutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *sls_objalloc(void const *prototype, size_t size)
{
  void *obj = NULL;
  obj = calloc(size, 1);
  sls_checkmem(obj);
  memcpy(obj, prototype, size);
  return obj;

  error:
  return NULL;
}

void sls_sleep(clock_t ticks)
{
  clock_t init_time = clock();
  while (clock() - init_time < ticks) { }
}
