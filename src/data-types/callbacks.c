/**
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea. 
**/

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
  sls_checkmem(str);
  char const *typed_str = str;
  char *cpy = NULL;
  size_t size = strnlen(typed_str, max_string_len);
  cpy = calloc(size+1, sizeof(char));
  sls_checkmem(cpy);

  strncpy(cpy, typed_str, size+1);
  cpy[size] = '\0';

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

int sls_cmp_voidptr(void const *a, void const *b)
{
  return (int)((const char*)b - (const char*)a);
}
