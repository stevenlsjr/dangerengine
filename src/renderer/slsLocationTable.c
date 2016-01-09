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
      .cmp_fn = sls_cmp_string,
      .copy_fn= sls_copy_string
  };
  slsCallbackTable uintptr_cback = {
      .free_fn = free,
      .cmp_fn = sls_cmp_uintptr
  };

  slsHashTable *ht =
      sls_hashtable_init(&self->ht, init_size, sls_hash_fn_cstr, &string_cback, &uintptr_cback);
  sls_checkmem(ht);


  return self;

  error:
  if (self) {
    sls_locationtable_dtor(self);
  }
  return self;

}

slsLocationTable *sls_locationtable_dtor(slsLocationTable *self)
{
  sls_hashtable_dtor(&self->ht);
  return self;
}

size_t * sls_locationtable_get(slsLocationTable *self, char const *name)
{
  slsHashValue *val = sls_hashtable_find(&self->ht, name, 0);
  if (val && val->type == SLS_SIZE_T) {
    return &val->size_tval;

  } else {
    return NULL;
  }
}

size_t const * sls_locationtable_set(slsLocationTable *self, char const *name, GLuint location)
{

  slsHashValue v = sls_size_t_hashvalue(location);

  slsHashValue const *res = sls_hashtable_insert(&self->ht, (void*)name, 0, &v);

  sls_check(res && res->type == SLS_SIZE_T, "invalid result");

  return &res->size_tval;
  error:
  return NULL;
}

void sls_locationtable_remove(slsLocationTable *self, char const *name)
{
  sls_hashtable_insert(&self->ht, strdup(name), 0, NULL);
}

GLuint sls_locationtable_get_val(slsLocationTable *self, char const *name)
{
  size_t const *ptr = sls_locationtable_get(self, name);
  return ptr ? (GLuint)*ptr : 0;
}
