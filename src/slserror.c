#include "slserror.h"

/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/7/17, Steven
 * 
 **/

struct slsErrorPrivate {
  bool is_error;
};

#undef static_assert
#define static_assert(...)


static_assert(sizeof(slsErrorPrivate) < SLS_ERROR_PIMPL_MAXSIZE,
  "slsErrorPrivate struct must be smaller than buffer allocated for it: ");


struct slsError *
sls_error_init(slsError *self, slsErrorRoom room, long code, char *description_alloced)
{
  self->pimpl = (slsErrorPrivate*)self->buffer;
  return self;
}

slsError *sls_error_init_noerror(slsError *self)
{
  self->pimpl = (slsErrorPrivate*)self->buffer;

  return self;
}

slsError *sls_error_dtor(slsError *self)
{
  return self;
}
