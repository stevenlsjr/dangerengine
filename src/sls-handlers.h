//
// Created by Steven on 5/2/15.
//

#ifndef DANGERENGINE_SLS_HANDLERS_H
#define DANGERENGINE_SLS_HANDLERS_H

#include <stdbool.h>
#include "slscontext.h"

void sls_bind_context(slsContext *ctx);
void sls_unbind_context(void);

bool sls_init(void);
void sls_terminate(void);

bool sls_is_active(void);


#endif //DANGERENGINE_SLS_HANDLERS_H
