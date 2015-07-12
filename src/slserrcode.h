//
// Created by Steven on 7/11/15.
//

#ifndef DANGERENGINE_SLSERRCODE_H
#define DANGERENGINE_SLSERRCODE_H

#include <err.h>
#include <errno.h>
#include <stdlib.h>

typedef enum slsError {
  SLS_OK = 0,
  SLS_MALLOC_ERR
} slsError;

void sls_push_error(slsError err);
slsError sls_geterr();
char const *sls_strerr(slsError err);
size_t sls_get_error_count();

#endif //DANGERENGINE_SLSERRCODE_H

