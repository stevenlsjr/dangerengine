/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/26/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSLOCATIONTABLE_H
#define DANGERENGINE_SLSLOCATIONTABLE_H

#include <data-types/hashtable.h>
#include <sls-gl.h>

typedef struct slsLocationTable slsLocationTable;

struct slsLocationTable {
  slsHashTable ht;
  slsArray values;
};

slsLocationTable *sls_locationtable_init(slsLocationTable *self);
slsLocationTable *sls_locationtable_dtor(slsLocationTable *self);

GLuint const * sls_locationtable_get(slsLocationTable *self, char const *name);
GLuint sls_locationtable_get_val(slsLocationTable *self, char const *name);
GLuint const * sls_locationtable_set(slsLocationTable *self, char const *name, GLuint location);

void sls_locationtable_remove(slsLocationTable *self, char const *name);

#endif //DANGERENGINE_SLSLOCATIONTABLE_H
