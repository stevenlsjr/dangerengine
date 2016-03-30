//
// Created by Steven on 7/22/15.
//

/**
 * @file ptrarray.h
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

#ifndef DANGERENGINE_PTRARRAY_H
#define DANGERENGINE_PTRARRAY_H

#include <stdlib.h>

#include "callbacks.h"

typedef struct slsPtrArray slsPtrArray;

/**
 * @brief A dynamic array specifically for storing an array of pointers
 */
struct slsPtrArray {
  void **data;
  size_t n_elements;
  size_t n_alloced;

  slsFreeFn free_fn;

};

slsPtrArray *sls_ptrarray_init(slsPtrArray *self,
                               void **data, size_t
                               n_elements,
                               slsFreeFn free_fn);

slsPtrArray *sls_ptrarray_dtor(slsPtrArray *self);


void sls_ptrarray_reserve(slsPtrArray *self, size_t size);

void sls_ptrarray_insert(slsPtrArray *self, size_t idx, void *ptr);
void *sls_ptrarray_remove(slsPtrArray *self, size_t idx);
void sls_ptrarray_append(slsPtrArray *self, void *ptr);
void sls_ptrarray_prepend(slsPtrArray *self, void *ptr);


#define SLS_PTRARRAY_FOREACH(array, itor) \
  for ((itor) = (array)->data;  \
   itor != (array)->data + (array)->n_elements; \
    ++itor)



#endif //DANGERENGINE_PTRARRAY_H


