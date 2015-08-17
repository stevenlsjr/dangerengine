//
// Created by Steven on 7/2/15.
//

#include "hashtable.h"
#include "callbacks.h"
#include <stdlib.h>
#include <data-types/callbacks.h>

slsHashTable *sls_hashtable_init(slsHashTable *self, size_t n_entries, slsHashFn hash_fn, slsCmpFn cmp_fn)
{
  if (!self) { return NULL; }
  return self;
}

slsHashTable *sls_hashtable_dtor(slsHashTable *self)
{
  if (self) {
    if (self->entries) {

    }
  }
  return self;
}