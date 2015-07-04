//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_HASHTABLE_H
#define DANGERENGINE_HASHTABLE_H


#include <stddef.h>
#include <uthash.h>
#include "callbacks.h"

typedef struct slsHashtable slsHashtable;
typedef struct slsTableEntry slsTableEntry;


struct slsTableEntry {
  void *key;
  void *val;

  UT_hash_handle hh;
};

struct slsHashtable {


  size_t table_size;
  slsTableEntry *table;

  slsCallbackTable key_callbacks;
  slsCallbackTable val_callbacks;
};

slsCallbackTable const *sls_string_callbacks();

slsCallbackTable const *sls_int_callbacks();

slsHashtable *sls_hashtable_new(size_t reserve_size,
                                slsCallbackTable const *val_table,
                                slsCallbackTable const *key_table);


#endif //DANGERENGINE_HASHTABLE_H


