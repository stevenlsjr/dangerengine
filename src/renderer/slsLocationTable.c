/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/26/15, Steven
 * 
 **/
#include "slsLocationTable.h"

slsLocationTable *sls_locationtable_init(slsLocationTable *self)
{
  const size_t init_size = 32;

  slsCallbackTable string_cback = {
      .free_fn = free,
      .cmp_fn = sls_cmp_string
  };
  slsCallbackTable uintptr_cback = {
      .cmp_fn = sls_cmp_uintptr
  };

  slsHashTable *ht =
      sls_hashtable_init(&self->ht, init_size, sls_hash_fn_cstr, &string_cback, &uintptr_cback);
  sls_checkmem(ht);

  slsArray *arr = sls_array_init(&self->values, (GLuint[]){}, sizeof(GLuint), 0);
  sls_checkmem(arr);


  return self;

  error:
  if (self) {
    sls_locationtable_dtor(self);
  }
  return self;

}

slsLocationTable *sls_locationtable_dtor(slsLocationTable *self)
{
  sls_array_dtor(&self->values);
  sls_hashtable_dtor(&self->ht);
  return self;
}

GLuint const * sls_locationtable_get(slsLocationTable *self, char const *name)
{
  return sls_hashtable_find(&self->ht, name, 0);
}

GLuint const * sls_locationtable_set(slsLocationTable *self, char const *name, GLuint location)
{
  GLuint *res = sls_hashtable_find(&self->ht, name, 0);
  if (res) {
    *res = location;
  } else {
    char *buffer = NULL;
    buffer = strdup(name);
    sls_checkmem(buffer);

    sls_array_append(&self->values, &location);
    GLuint  *ptr = sls_array_get(&self->values, sls_array_length(&self->values) - 1);
    sls_hashtable_insert(&self->ht, buffer, 0, ptr);
    res = ptr;

  }
  return res;

  error:
    return NULL;
}

void sls_locationtable_remove(slsLocationTable *self, char const *name)
{
  sls_hashtable_insert(&self->ht, strdup(name), 0, NULL);
}

GLuint sls_locationtable_get_val(slsLocationTable *self, char const *name)
{
  GLuint const *ptr = sls_locationtable_get(self, name);
  return ptr? *ptr: 0;
}
