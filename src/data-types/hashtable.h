//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_HASHTABLE_H
#define DANGERENGINE_HASHTABLE_H

#include <stdlib.h>


typedef struct slsHashTable slsHashTable;
typedef struct slsTableEntry slsTableEntry;

struct slsTableEntry{
  void *key;
  void *val;
};


struct slsHashTable{
  slsTableEntry *entries;
  size_t n_entries;



};

slsHashTable *sls_hashtable_init(slsHashTable *self, size_t n_entries);
slsHashTable *sls_hashtable_dtor(slsHashTable *self);





#endif //DANGERENGINE_HASHTABLE_H



