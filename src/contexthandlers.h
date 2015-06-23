/**
 * Created by Steven on 5/2/15.
 * @file contexthandlers.h
 * @brief functions for setting up glfw context.
 */

#ifndef DANGERENGINE_SLS_HANDLERS_H
#define DANGERENGINE_SLS_HANDLERS_H

#include <stdbool.h>
#include "slscontext.h"

/**
 * @brief set slsContext object as current render target
 */
void sls_bind_context(slsContext *ctx);
void sls_unbind_context(void);

/**
 * @brief initializes runtime libraries
 */
bool sls_init(void);

/**
 * @brief terminates runtime libraries
 */
void sls_terminate(void);

bool sls_is_active(void);


#endif //DANGERENGINE_SLS_HANDLERS_H
