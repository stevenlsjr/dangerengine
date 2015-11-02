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
#include <assert.h>
#include <string.h>
#include <tgmath.h>




void sls_hashtable_insert_with_hash(slsHashTable *self, void *key, void *val, uint64_t hash);

slsHashTable *sls_hashtable_init(slsHashTable *self, size_t array_size, slsHashFn hash_fn,
                                 slsCallbackTable const *key_cback, slsCallbackTable const *val_cback)
{

  *self = (slsHashTable) {
      .vals = NULL,
      .keys = NULL,
      .array_size = array_size,
      .n_entries = 0,
      .hash = hash_fn,
      .key_callbacks = (key_cback) ? *key_cback : (slsCallbackTable) {},
      .val_callbacks = (val_cback) ? *val_cback : (slsCallbackTable) {}};

  self->hashes = calloc(array_size, sizeof(uint64_t));
  self->keys = calloc(array_size, sizeof(void *));
  self->vals = calloc(array_size, sizeof(void *));

  sls_checkmem(self->hashes);
  sls_checkmem(self->keys);
  sls_checkmem(self->vals);

  if (!self->key_callbacks.cmp_fn) {
    self->key_callbacks.cmp_fn = sls_cmp_voidptr;
  }
  if (!self->val_callbacks.cmp_fn) {
    self->key_callbacks.cmp_fn = sls_cmp_voidptr;
  }

  if (!hash_fn) {
    self->hash = sls_hash_fn_default;
  }


  return self;

  error:

  return sls_hashtable_dtor(self);
}

slsHashTable *sls_hashtable_dtor(slsHashTable *self)
{
  if (self->vals) {
    slsFreeFn free_fn = self->val_callbacks.free_fn;
    if (free_fn) {
      for (int i = 0; i < self->array_size; ++i) {
        void *v = self->vals[i];
        if (v) { free_fn(v); }
      }
    }
    free(self->vals);
  }

  if (self->keys) {
    slsFreeFn free_fn = self->key_callbacks.free_fn;
    if (free_fn) {
      for (int i = 0; i < self->array_size; ++i) {

        void *key = self->keys[i];
        if (key) { free_fn(key); }
      }
    }
    free(self->keys);
  }

  if (self->hashes) {
    free(self->hashes);
  }


  return self;
}


void sls_hashtable_reserve(slsHashTable *self, size_t n_items)
{

  if (n_items < self->n_entries) {
    n_items = self->n_entries * 2;
  }

  size_t old_size = self->array_size;

  uint64_t *hashes = self->hashes;
  void **keys = self->keys;
  void **vals = self->vals;
  sls_checkmem(hashes);
  sls_checkmem(keys);
  sls_checkmem(vals);


  self->keys = calloc(n_items, sizeof(void**));
  self->vals = calloc(n_items, sizeof(void**));
  self->hashes = calloc(n_items, sizeof(uint64_t));

  sls_checkmem(self->hashes);
  sls_checkmem(self->keys);
  sls_checkmem(self->vals);

  for (int i=0; i<old_size; ++i) {
    if (keys[i] && vals[i] && !sls_is_hash_sentinel(keys[i]))
    sls_hashtable_insert_with_hash(self, keys[i], vals[i], hashes[i]);
  }
  return;

  error:
    return;

}

void sls_hashtable_insert(slsHashTable *self, void *key, size_t key_size, void *val)
{
  const size_t array_size = self->array_size;
  uint64_t hash = self->hash(key, key_size);

  sls_hashtable_insert_with_hash(self, key, val, hash);

}


void sls_hashtable_insert_with_hash(slsHashTable *self,
                                    void *key,
                                    void *val,
                                    uint64_t hash)
{
  const size_t array_size = self->array_size;

  if (self->n_entries == array_size) {
    sls_hashtable_reserve(self, array_size * 2);
  }

  bool inserted = false;

  for (size_t i = 0; !inserted; ++i) {
    size_t probe = hash + (i * i);
    size_t idx = probe % array_size;
    void **k_itor = self->keys + idx;
    void **v_itor = self->vals + idx;

    if (*k_itor && self->key_callbacks.cmp_fn(*k_itor, key) == 0) {
      if (self->key_callbacks.free_fn) {
        self->key_callbacks.free_fn(*k_itor);
      }
      if (self->val_callbacks.free_fn) {
        self->val_callbacks.free_fn(*v_itor);
      }
      *k_itor = NULL;
    }

    if (!*k_itor) {
      *k_itor = self->key_callbacks.copy_fn ?
                self->key_callbacks.copy_fn(key) :
                key;
      *v_itor = self->val_callbacks.copy_fn?
                self->val_callbacks.copy_fn(val):
                val;

      inserted = true;
    }
  }

  self->n_entries++;
}

void *sls_hashtable_find(slsHashTable *self, void const *key, size_t key_size)
{
  void *ptr = NULL;
  sls_check(self->keys, "no val array");

  assert(self->key_callbacks.cmp_fn);
  size_t array_size = self->array_size;

  uint64_t hash = self->hash(key, key_size);

  bool found = false;

  for (size_t i = 0; !found && i < array_size; ++i) {
    size_t probe = hash + (i * i);
    size_t idx = probe % array_size;
    void **k_itor = self->keys + idx;
    void **v_itor = self->vals + idx;

    if (*k_itor && self->key_callbacks.cmp_fn(*k_itor, key) == 0) {
      ptr = *v_itor;
      found = true;
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

  assert(self->val_callbacks.cmp_fn);
  slsCmpFn cmp = self->val_callbacks.cmp_fn;

  for (int i = 0; i < self->array_size; ++i) {
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


static int sls_hash_sentinel_value = 0;

bool sls_is_hash_sentinel(void const *val)
{
  return val == sls_hash_sentinel();
}

void const *sls_hash_sentinel()
{
  return &sls_hash_sentinel_value;
}

uint64_t sls_hash_fn_default(void const *val, size_t size)
{
  char *buffer = malloc(size + 1);
  memcpy(buffer, val, size);

  // TODO: implement hash algorithm

  free(buffer);
  return 0;
}

