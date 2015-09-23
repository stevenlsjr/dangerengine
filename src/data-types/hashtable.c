//
// Created by Steven on 7/2/15.
//

/**
 * @file hashtable.c
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

#include "hashtable.h"
#include "callbacks.h"
#include <stdlib.h>
#include <data-types/callbacks.h>

slsHashTable *sls_hashtable_init(slsHashTable *self,
                                 size_t array_size,
                                 slsHashFn hash_fn,
                                 slsCmpFn cmp_fn,
                                 slsCallbackTable const *key_cback,
                                 slsCallbackTable const *val_cback)
{

  *self = (slsHashTable) {
      .vals = NULL,
      .keys = NULL,
      .array_size = array_size,
      .n_entries = 0,
      .hash = hash_fn,
      .cmp = cmp_fn,
      .key_callbacks = (key_cback) ? *key_cback : (slsCallbackTable){},
      .val_callbacks = (val_cback) ? *val_cback : (slsCallbackTable){}};

  self->keys = calloc(array_size, sizeof(slsKeyEntry));
  self->vals = calloc(array_size, sizeof(void*));

  sls_checkmem(self->keys);
  sls_checkmem(self->vals);


  return self;

  error:

    return sls_hashtable_dtor(self);
}

slsHashTable *sls_hashtable_dtor(slsHashTable *self)
{
  if (self->vals) {
    slsFreeFn free_fn = self->val_callbacks.free_fn;
    if (free_fn) {
      for (int i=0; i<self->array_size; ++i) {
        void *v = self->vals[i];
        if (v) {free_fn(v);}
      }
    }
    free(self->vals);
  }

  if (self->keys) {
    slsFreeFn free_fn = self->key_callbacks.free_fn;
    if (free_fn) {
      for (int i=0; i<self->array_size; ++i) {
        void *key = self->keys[i].ptr;
        if (key) {free_fn(key);}
      }
    }

    free(self->keys);
  }



  return self;
}




void sls_hashtable_reserve(slsHashTable *self, size_t n_items)
{
  if (self->n_entries >= n_items) { return; }


}

void sls_hashtable_insert(slsHashTable *self, void *key, void *val)
{

}

void *sls_hashtable_find(slsHashTable *self, void const *key)
{
  void *ptr = NULL;
  sls_check(self->keys, "no val array");


  slsCmpFn cmp = self->key_callbacks.cmp_fn?
                 self->key_callbacks.cmp_fn:
                 sls_cmp_voidptr;

  for (int i=0; i<self->array_size; ++i) {
    slsKeyEntry *iter = self->keys + i;

    if (iter->is_occupied && // key must have occupied flag
        iter->ptr && // ptr must be nonnull
        cmp(iter->ptr, key) == 0) {
      ptr = iter->ptr;
      return ptr;
    }
  }
  return ptr;

  error:
  return ptr;
}

void *sls_hashtable_findval(slsHashTable *self, void const *val)
{

  void *ptr = NULL;
  sls_check(self->vals, "no val array");


  slsCmpFn cmp = self->val_callbacks.cmp_fn?
                 self->val_callbacks.cmp_fn:
                 sls_cmp_voidptr;

  for (int i=0; i<self->array_size; ++i) {
    void *iter = self->vals[i];
    if (iter && cmp(iter, val) == 0) {
      ptr = iter;
      return ptr;
    }
  }
  return ptr;

  error:
  return ptr;
}



