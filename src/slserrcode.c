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
  typedef struct err_pair_s {
    slsError key;
    char *value;
  } err_pair_t;

  static const err_pair_t pairs[] = {
      {SLS_OK, "SLS_OK, no error"},
      {SLS_MALLOC_ERR, "SLS_MALLOC_ERR, malloc error"},
      {SLS_INDEX_OVERFLOW, "SLS_INDEX_OVERFLOW, index overflow"}
  };

  static char *unknown = "unknown error";

  char *rval = NULL;
  const size_t len = sizeof(pairs)/sizeof(err_pair_t);
  for (int i=0; i<len; ++i) {
    if (err == pairs[i].key) {
      rval = pairs[i].value;
      break;
    }
  }

  if (!rval) {
    rval = unknown;
  }


  return rval;
}

size_t sls_get_error_count()
{
  return error_stack_count;
}
