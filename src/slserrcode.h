/// @file "slserrcode.h"
/// Created by Steven on 7/11/15.
///

#ifndef DANGERENGINE_SLSERRCODE_H
#define DANGERENGINE_SLSERRCODE_H

#include <err.h>
#include <errno.h>
#include <stdlib.h>

typedef enum slsError {
  SLS_OK = 0,
  SLS_MALLOC_ERR,
  SLS_INDEX_OVERFLOW
} slsError;

void sls_setup_errstack();

void sls_teardown_errstack();

/**
 * @brief reports an error by pusing it onto the errorstack.
 */
void sls_push_error(slsError err);

/**
 * @brief Pops the top error from the stack and returns it
 */
slsError sls_geterr();

/**
 * @brief returns the name of an error code as a static string
 * @return a string corresponding to the given error. It is statically
 * allocated and must not be freed by user
 */
char const *sls_strerr(slsError err);

/**
 * @brief returns number of errors on the stack
 */
size_t sls_get_error_count();

#endif //DANGERENGINE_SLSERRCODE_H

