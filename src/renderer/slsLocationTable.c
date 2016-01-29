/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/26/15, Steven
 * 
 **/
#include "slsLocationTable.h"

static
void *sls_dup_size_t(void const *size_t_ptr)
{
  size_t const *p = size_t_ptr;
  assert(p);
  size_t *res = malloc(sizeof(size_t));

  *res = *p;
  return res;

}

static int sls_cmp_size_t(void const *lhs, void const *rhs)
{
  assert(lhs && rhs);
  size_t const *tlhs = lhs;
  size_t const *trhs = rhs;
  if (*tlhs > *trhs) {return -1;}
  else if (*tlhs < *trhs) {return 1;}
  else {return 0;}


}

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
      .cmp_fn = sls_cmp_size_t,
      .copy_fn= sls_dup_size_t
  };

  self->ht = (slsHashTable){};
  slsHashTable *ht =
      sls_hashtable_init(&self->ht,
                         init_size,
                         sls_hash_fn_default, &string_cback, &uintptr_cback);
  sls_checkmem(ht);


  return self;

  error:
  SLS_DEBUGGER_BREAKPT();
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
  size_t *val = sls_hashtable_find(&self->ht, name, SLS_STRING_LENGTH);
  return val;
}

size_t const * sls_locationtable_set(slsLocationTable *self, char const *name, GLuint location)
{

  size_t t_loc = location;
  size_t const *res = sls_hashtable_insert(&self->ht, name, SLS_STRING_LENGTH, &t_loc);

  sls_check(res, "null result");
  sls_check(*res == location, "resulting value does not equal location parameter");

  return res;
  error:

  return NULL;
}

void sls_locationtable_remove(slsLocationTable *self, char const *name)
{
  sls_hashtable_remove(&self->ht, (void*)name, SLS_STRING_LENGTH);
}

GLuint sls_locationtable_get_val(slsLocationTable *self, char const *name)
{
  size_t const *ptr = sls_locationtable_get(self, name);
  return ptr ? (GLuint)*ptr : 0;
}
