/**
 * // Created by Steven on 5/3/15.
 * @file data-types/array.h
 * @brief __
 */

/**
 * @file array.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#ifndef DANGERENGINE_ARRAY_H
#define DANGERENGINE_ARRAY_H

#include <stddef.h>

#include "callbacks.h"
#include <slsutils.h>
#include <stdlib.h>

typedef struct slsArray slsArray;
typedef struct slsArray_p slsArray_p;

typedef struct slsArrayItor slsArrayItor;

struct slsArrayItor {
  slsArray* array;

  void* elt;
  size_t index;
};

struct slsArray {

  /**
   * @brief initializer
   *
   * @param data array of elements.
   */
  slsArray* (*init)(slsArray* self,
                    void const* data,
                    size_t esize,
                    size_t n_elements);

  slsArray* (*dtor)(slsArray* self);

  slsCallbackTable table;

  slsArray_p* priv;
};

slsArray const* sls_array_class();

slsArray* sls_array_new(void const* array,
                        size_t element_size,
                        size_t n_elements);

size_t sls_array_length(slsArray const* self);

size_t sls_array_element_size(slsArray const* self);

void* sls_array_get(slsArray* self, size_t i);

/**
 * @brief Const-correct variation of sls_array_get
 */
void const* sls_array_cget(slsArray const* self, size_t i);

/**
 * @brief sets the value of a current element
 * @details [long description]
 *
 * @param t [description]
 * @param i [description]
 * @param value [description]
 * @return [description]
 */
void sls_array_set(slsArray* self, size_t i, void* value);

size_t sls_array_alloc_size(slsArray* self);

void sls_array_reserve(slsArray* self, size_t count);

/**
 * @brief inserts an element, pushing back elements ahead in the array.
 * @details [long description]
 *
 * @param i index
 * @param value pointer to value
 */
void sls_array_insert(slsArray* self, size_t i, void* value);

void sls_array_insert_array(slsArray* self, size_t i, const void* data);

void sls_array_append(slsArray* self, void* value);

slsArray* sls_array_copy(slsArray const* self);

slsArrayItor* sls_arrayitor_begin(slsArray* self, slsArrayItor* itor)
  SLS_NONNULL(1, 2);

slsArrayItor* sls_arrayitor_next(slsArrayItor* self) SLS_NONNULL(1);

slsArray* sls_array_init(slsArray* self,
                         void const* data,
                         size_t element_size,
                         size_t n_elements);

slsArray* sls_array_dtor(slsArray* self);

/**
 * @brief convenience method for retrieving index values.
 * @detail WARNING: dereferences pointer given by slsArray::get
 * will dereference a NULL value.
 */
#define SLS_ARRAY_IDX(array, T, idx) (*((T*)(sls_array_get((array), (idx)))))

#define SLS_ARRAY_FOREACH(array, itor)                                         \
  for ((itor) = sls_arrayitor_begin((array), (itor)); (itor);                  \
       (itor) = sls_arrayitor_next((itor)))

#endif // DANGERENGINE_ARRAY_H
