/**
 * //
 * // Created by Steven on 4/25/15.
 * //
 * 
 * @file
 * @brief utilities specific to Dangerengine environment
 */




#ifndef DANGERENGINE_SLSUTILS_H
#define DANGERENGINE_SLSUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define SLS_USE_ERRORSTACK 1
#include "slsmacros.h"

#include "slserrcode.h"


/**
 * @brief Error checker with same format as sls_check, but submits
 * a given error code on failure
 */
#define sls_check_code(cond, code, msg, ...) do  {\
  if (!(cond)) {                                  \
    sls_log_err((msg), ##__VA_ARGS__);            \
    sls_push_error((slsError)(code));             \
    goto error;                                   \
  }                                               \
} while (0)

// redefine
#undef sls_checkmem
#define sls_checkmem(pointer) do {  \
  sls_check_code((pointer), SLS_MALLOC_ERR, "memory error! %s", #pointer); \
} while (0)



typedef enum slsBool {
  SLS_TRUE = true,
  SLS_FALSE = false
} slsBool SLS_DEPRECIATED;


/**
 * @brief allocates 'size' bytes on the heap, and copies contents to
 * pointer
 * @details essentially a heap allocated memcpy, this facilitates
 * struct object construction
 * 
 * @param prototype [description]
 * @param size [description]
 * 
 * @return [description]
 */
void *sls_objalloc(void const *prototype, size_t size);

/**
 * @brief cross-platform chdir function.
 * @detail Calls either win32 or possix chdir function
 * and returns value as specified by given function
 */

int sls_chdir(char const *path) SLS_NONNULL(1);




#endif //DANGERENGINE_SLSUTILS_H
