//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_HASHTABLE_H
#define DANGERENGINE_HASHTABLE_H

#include <stdlib.h>
#include <data-types/array.h>
#include <data-types/callbacks.h>
#include "../slsutils.h"
#include "callbacks.h"


typedef struct slsHashTable slsHashTable;
typedef struct slsTableEntry slsTableEntry;


struct slsTableEntry{
  void *key;
  void *val;
  uint32_t hash;
};



struct slsHashTable{
  slsArray *entries;


  slsHashFn hash;
  slsCmpFn cmp;
};

slsHashTable *sls_hashtable_init(slsHashTable *self,
                                 size_t n_entries,
                                 slsHashFn hash_fn,
                                 slsCmpFn cmp_fn);

slsHashTable *sls_hashtable_dtor(slsHashTable *self);

void sls_hashtable_insert(slsHashTable *self, void *key, void *val);





#endif //DANGERENGINE_HASHTABLE_H



