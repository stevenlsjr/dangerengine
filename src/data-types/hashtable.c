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


#include <uthash.h>


struct slsHashtable_p {

};

static size_t _n_insertions = 0;
static size_t _n_collisions = 0;


slsHashValue const *sls_hashtable_insert_with_hash(slsHashTable *self, void *key, slsHashValue const *val,
                                                   uint64_t hash);


void sls_hashvalue_free(slsHashValue *val, slsFreeFn free_fn) SLS_NONNULL(1, 2);

slsHashValue sls_hashval_cpy(slsHashValue const *source, slsCopyFn copy_fn);

/**
 * @brief hashes a null-terminated string
 * @detail, to use as a hash parameter, cal sls_hash_fn_default
 * with length of SLS_STRING_LEN (0)
 *
 */


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

  if (array_size < 8) {
    array_size = SLS_DEFAULT_HT_CAPACITY;
  }

  if (key_cback){self->key_callbacks = *key_cback;}
  if (val_cback){self->val_callbacks = *val_cback;}

  self->hashes = calloc(array_size, sizeof(uint64_t));
  self->keys = calloc(array_size, sizeof(self->keys[0]));
  self->vals = calloc(array_size, sizeof(self->vals[0]));

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
        slsHashValue v = self->vals[i];
        // only release pointer values
        if (v.type == SLS_VOIDPTR) { free_fn(v.ptrvalue); }
      }
    }
    free(self->vals);
  }

  if (self->keys) {
    slsFreeFn free_fn = self->key_callbacks.free_fn;
    if (free_fn) {
      for (int i = 0; i < self->array_size; ++i) {

        void *key = self->keys[i];
        if (key && free_fn) { free_fn(key); }
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

  typeof(self->hashes) hashes = self->hashes;
  typeof(self->keys) keys = self->keys;
  typeof(self->vals) vals = self->vals;
  sls_checkmem(hashes);
  sls_checkmem(keys);
  sls_checkmem(vals);


  self->keys = calloc(n_items, sizeof(self->keys[0]));
  self->vals = calloc(n_items, sizeof(self->vals[0]));
  self->hashes = calloc(n_items, sizeof(self->hashes[0]));

  sls_checkmem(self->hashes);
  sls_checkmem(self->keys);
  sls_checkmem(self->vals);

  for (int i = 0; i < old_size; ++i) {
    if (keys[i] && vals[i].is_active)
      sls_hashtable_insert_with_hash(self, keys[i], vals + i, hashes[i]);
  }
  return;

  error:
  return;

}

slsHashValue const *sls_hashtable_insert(slsHashTable *self, void *key, size_t key_size, slsHashValue const *val)
{
  const size_t array_size = self->array_size;
  uint64_t hash = self->hash(key, key_size);

  return sls_hashtable_insert_with_hash(self, key, val, hash);
}


slsHashValue const *sls_hashtable_insert_with_hash(slsHashTable *self,
                                                   void *key,
                                                   slsHashValue const *val,
                                                   uint64_t hash)
{
  assert(self);
  assert(self->keys);
  assert(self->vals);
  const size_t array_size = self->array_size;
  slsHashValue *res = NULL;

  slsCmpFn cmpFn = self->key_callbacks.cmp_fn ?
                   self->key_callbacks.cmp_fn : sls_cmp_voidptr;

  if (self->n_entries == array_size) {
    sls_hashtable_reserve(self, array_size * 2);
  }

  bool inserted = false;

  for (size_t i = 0; !inserted; ++i) {

    size_t probe = sls_hashtable_probe_next(self, hash, i, NULL);


    void **pkey = self->keys + probe;
    slsHashValue *pval = self->vals + probe;

    if (!*pkey && !sls_is_hash_sentinel(*pkey)) {
      inserted = true;
    } else if (cmpFn(key, self->keys[i]) == 0) {
      // replace existing key if already exists
      if (self->key_callbacks.free_fn) {
        self->key_callbacks.free_fn(self->keys[i]);
      }

      if (self->key_callbacks.free_fn) {
        self->key_callbacks.free_fn(*pkey);
      }
      if (self->val_callbacks.free_fn) {
        self->val_callbacks.free_fn(val->ptrvalue);
        self->val_callbacks.free_fn(val->ptrvalue);
      }
      self->n_entries--;
      inserted = true;
    }

    if (inserted) {
      if (self->key_callbacks.copy_fn) {
        *pkey = self->key_callbacks.copy_fn(key);
      } else { *pkey = key; }

      *pval = *val;
      pval->is_active = true;
      if (self->val_callbacks.copy_fn) {
        pval->ptrvalue = self->val_callbacks.copy_fn(val->ptrvalue);
      }

      self->hashes[probe] = hash;
      res = pval;
    }
  }

  self->n_entries++;
  return res;
}

slsHashValue sls_hashval_cpy(slsHashValue const *source, slsCopyFn copy_fn)
{
  slsHashValue result = *source;

  if (source->type == SLS_VOIDPTR && copy_fn) {
    result.ptrvalue = copy_fn(source->ptrvalue);
  }

  return result;
}

void sls_hashvalue_free(slsHashValue *val, slsFreeFn free_fn)
{

  if (val->type == SLS_VOIDPTR) {
    free_fn(val->ptrvalue);
  }
}

slsHashValue *sls_hashtable_find(slsHashTable *self, void const *key, size_t key_size)
{
  slsHashValue *ptr = NULL;
  sls_check(self->keys, "no val array");

  assert(self->key_callbacks.cmp_fn);
  size_t array_size = self->array_size;

  //slsCmpFn cmp = self->key_callbacks.cmp_fn;
  slsCmpFn cmp = sls_cmp_string;

  uint64_t hash = self->hash(key, key_size);

  bool found = false;

  for (size_t i = 0; !found; ++i) {
    size_t probe = sls_hashtable_probe_next(self, hash, i, NULL);
    assert(probe <= self->array_size);
    void **pkey = self->keys + probe;

    if (sls_is_hash_sentinel(*pkey)) { continue; }
    if (!(*pkey)) {
      // key doesn't exist
      ptr = NULL;
      found = true;
    }

    int res = cmp(*pkey, key);
    if (res == 0) {
      ptr = *pkey;
      found = true;
    }
  }

  return ptr;

  error:
  return ptr;
}

slsHashValue const *sls_hashtable_findval(slsHashTable *self, slsHashValue const *val)
{

  void *ptr = NULL;
  sls_check(self->vals, "no val array");

  assert(self->val_callbacks.cmp_fn);
  slsCmpFn cmp = self->val_callbacks.cmp_fn;

  for (int i = 0; i < self->array_size; ++i) {
    slsHashValue *iter = self->vals + i;
    if (iter && sls_hashvalue_cmp(val, iter, cmp) == 0) {
      ptr = iter;
      return ptr;
    }
  }
  return ptr;

  error:
  return ptr;
}


//---------------------------------hashtable probing sentinel values---------------------------------------
static int sls_hash_sentinel_value = 0;

bool sls_is_hash_sentinel(void const *val)
{
  return val == sls_hash_sentinel();
}

void const *sls_hash_sentinel()
{
  return &sls_hash_sentinel_value;
}

static inline
void *sls_hash_sentinel_mut()
{
  return &sls_hash_sentinel_value;
}

uint64_t sls_hash_fn_naive(void const *val, size_t size)
{
  char const *buffer = val;
  // TODO: implement hash algorithm
  uint64_t checksum = 0;
  size_t max_strlen = 1000; // only hash limited characters
  bool is_cstr = (size == SLS_STRING_LENGTH);

  for (int i = 0;
       i < size / sizeof(char) || (is_cstr && buffer[i] != '\0');
       ++i) {
    checksum += buffer[i];
  }

  return checksum;
}

uint64_t sls_hash_fn_default(void const *val, size_t size)
{
  // TODO: implement hash algorithm

  if (size == SLS_STRING_LENGTH) {
    return sls_hash_cstr(val);
  } else {

    return sls_hash_sizeddata(val, size);

  }
}

uint64_t sls_hash_sizeddata(void const *val, size_t size)
{
  char const *buffer = val;

  // Jenkins Hash
  uint64_t hash = 0;

  for (int i = 0;
       i < size / sizeof(char);
       ++i) {
    hash += buffer[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

uint64_t sls_hash_cstr(char const *str)
{

  // K&R-style multiplicitive hash function
  const uint64_t initial_hash = 0;
  const int a = 31;
  uint64_t hash = initial_hash;

  size_t max_len = 1000;
  for (int i = 0;
       str[i] != '\0' && i < max_len;
       ++i) {
    hash = ((hash * a) + str[i]) % UINT64_MAX;
  }

  return hash;
}

void sls_hashtable_remove(slsHashTable *self, void *key, size_t key_size)
{
  slsHashValue *val = sls_hashtable_find(self, key, key_size);
  if (val && !sls_is_hash_sentinel(val)) {
    if (self->val_callbacks.free_fn && val->type == SLS_VOIDPTR) {
      self->val_callbacks.free_fn(val->ptrvalue);
      val->is_active = false;
    }
  }

  size_t idx = val - self->vals;
  self->keys[idx] = sls_hash_sentinel_mut();

}

slsHashItor *sls_hashitor_first(slsHashTable *table, slsHashItor *itor)
{
  int first = -1;
  itor->table = table;
  for (int i = 0; i < table->array_size; ++i) {
    if (table->keys[i] && table->vals[i].is_active) {
      first = i;
      itor->index = (size_t) i;
      itor->key = table->keys + i;
      itor->val = table->vals + i;
      break;
    }
  }

  return first > -1 ?
         itor :
         NULL;
}

slsHashItor *sls_hashitor_next(slsHashItor *itor)
{
  sls_checkmem(itor->key && itor->table && itor->val);
  bool found_next = false;
  slsHashTable *table = itor->table;

  for (size_t i = itor->index; i < table->array_size; ++i) {
    if (table->keys[i] && table->vals[i].is_active) {
      found_next = true;

      itor->index = i;
      itor->key = table->keys + i;
      itor->val = table->vals + i;

      break;
    }
  }

  return found_next ?
         itor :
         NULL;

  error:
  return NULL;
}

uint64_t sls_hash_fn_cstr(void const *cstr, size_t size)
{
  return sls_hash_cstr(cstr);
}

void _sls_hashtable_profile(size_t *n_collisions, size_t *total_insertions)
{
  if (n_collisions) {
    *n_collisions = _n_collisions;
  }
  if (total_insertions) {
    *total_insertions = _n_insertions;
  }
}

int sls_hashvalue_cmp(slsHashValue const *a, slsHashValue const *b, slsCmpFn ptr_cmp)
{
  if (a->type != b->type) { return -1; }
  int res = -1;
  switch (a->type) {

    case SLS_VOIDPTR: {
      if (ptr_cmp) { res = ptr_cmp(a->ptrvalue, b->ptrvalue); }
      else { sls_cmp_voidptr(a->ptrvalue, b->ptrvalue); }
    }
      break;
    case SLS_INT64:
      res = (int) (a->int64val - b->int64val);
      break;
    case SLS_DOUBLE: {
      double aa = a->doubleval;
      double bb = b->doubleval;
      if (aa == bb) {
        res = 0;
      } else if (aa < bb) {
        res = 1;
      } else {
        res = -1;
      }
    }
      break;
    case SLS_SIZE_T:
      res = (int) (a->size_tval - b->size_tval);
      break;
    case SLS_HASHDATATYPE_END:
      assert(!"invalid hashvalue type");
      res = -1;
      break;
    default:
      break;
  }
  return res;
}

size_t sls_hashtable_probe_next(slsHashTable const *table, size_t hashvalue, size_t i, bool *location_found)
{

  hashvalue = hashvalue % table->array_size;
  size_t probe = (hashvalue + i * i) % table->array_size;

  assert(probe <= table->array_size);
  return probe;
}
