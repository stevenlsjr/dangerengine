/**
 * Created by Steven on 5/2/15.
 * @file contexthandlers.h
 * @brief functions for setting up SDL2 context.
 */



#ifndef DANGERENGINE_SLS_HANDLERS_H
#define DANGERENGINE_SLS_HANDLERS_H

#include "slscontext.h"
#include <stdbool.h>

/**
 * @brief initializes runtime libraries
 */
bool
sls_init(void);

/**
 * @brief terminates runtime libraries
 */
void
sls_terminate(void);

bool
sls_is_active(void);

#endif // DANGERENGINE_SLS_HANDLERS_H
