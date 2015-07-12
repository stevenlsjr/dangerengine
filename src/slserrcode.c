//
// Created by Steven on 7/11/15.
//

#include "slserrcode.h"
#include <stdlib.h>
#include "slsutils.h"

#define SLS_ERRSTACK_MAXSIZE 100

static slsError error_stack[SLS_ERRSTACK_MAXSIZE] = {};
static size_t error_stack_count = 0;

void sls_push_error(slsError err)
{
  if (error_stack_count > SLS_ERRSTACK_MAXSIZE) {
    sls_log_err("error-code stack overflow!");
    error_stack_count = 0; // clear stack
  }

  // push code to top of stack
  error_stack[error_stack_count] = err;

  ++error_stack_count;



}

slsError sls_geterr()
{
  if (error_stack_count < 1) {
    return SLS_OK;
  } else {
    --error_stack_count;
    return error_stack[error_stack_count];
  }
}

char const *sls_strerr(slsError err)
{
  static char const *sls_ok = "SLS_OK, no error";
  static char const *sls_malloc_err = "SLS_MALLOC_ERR, malloc error";
  static char const *sls_unknown = "unknown error code";


  char const *rval = NULL;
  switch (err) {
    case SLS_OK:
      rval = sls_ok;
      break;
    case SLS_MALLOC_ERR:
      rval = sls_malloc_err;
      break;

    default: rval = sls_unknown;
  }


  return rval;
}

size_t sls_get_error_count()
{
  return error_stack_count;
}
