//
// Created by Steven on 7/2/15.
//

/**
 * @file hashtable.h
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
 *    and/or other materials provided with the distribution.
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

#ifndef DANGERENGINE_HASHTABLE_H
#define DANGERENGINE_HASHTABLE_H

#include <stdlib.h>
#include <data-types/array.h>
#include <data-types/callbacks.h>
#include "../slsutils.h"
#include "callbacks.h"
#include "ptrarray.h"


typedef struct slsHashTable slsHashTable;
typedef struct slsKeyEntry slsKeyEntry;

enum slsHashDataSize {
  SLS_STRING_LENGTH = 0,
  SLS_INT_LENGTH = sizeof(int),
  SLS_LONG_LENGTH = sizeof(long),
  SLS_PTR_LENGTH = sizeof(void*)
};

typedef uint64_t (*slsHashFn)(void const *key, size_t size);

/**
 * @brief Entry for a hash table key.
 * @detail Contains a void pointer as well as a sentinel
 * value determining if the table location is filled (in the event
 * of a key-value pair being removed)
 */

struct slsHashTable {
  uint64_t *hashes;
  void **keys;
  size_t *key_sizes;
  void **vals;

  /**
   * @brief the size of the hash table array;
   */
  size_t array_size;
  size_t n_entries;


  slsCallbackTable key_callbacks;
  slsCallbackTable val_callbacks;

  slsHashFn hash;
  slsCmpFn cmp;
};
typedef struct slsHashItor slsHashItor;


struct slsHashItor {
  void **key;
  void **val;
  size_t index;
  slsHashTable *table;
};

slsHashTable *sls_hashtable_init(slsHashTable *self,
                                 size_t array_size,
                                 slsHashFn hash_fn,
                                 slsCallbackTable const *key_cback,
                                 slsCallbackTable const *val_cback) SLS_NONNULL(1);

slsHashTable *sls_hashtable_dtor(slsHashTable *self) SLS_NONNULL(1);

void sls_hashtable_reserve(slsHashTable *self, size_t n_items) SLS_NONNULL(1);

void sls_hashtable_insert(slsHashTable *self, void *key, size_t key_size, void *val) SLS_NONNULL(1, 2, 4);
void sls_hashtable_remove(slsHashTable *self, void *key, size_t key_size) SLS_NONNULL(1);

void *sls_hashtable_find(slsHashTable *self, void const *key, size_t key_size) SLS_NONNULL(1, 2);

void *sls_hashtable_findval(slsHashTable *self, void const *val) SLS_NONNULL(1, 2);


bool sls_is_hash_sentinel(void const *val);
void const *sls_hash_sentinel();


uint64_t sls_hash_fn_default(void const *val, size_t size);

uint64_t sls_hash_cstr(char const *str);

uint64_t sls_hash_sizeddata(void const *val, size_t size);


slsHashItor *sls_hashitor_first(slsHashTable *table, slsHashItor *itor) SLS_NONNULL(1, 2);
slsHashItor *sls_hashitor_next(slsHashItor *itor) SLS_NONNULL(1);

#endif //DANGERENGINE_HASHTABLE_H



