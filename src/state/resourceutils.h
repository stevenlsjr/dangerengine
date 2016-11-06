/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/13/15, Steven
 * 
 **/
#ifndef DANGERENGINE_RESOURCEUTILS_H
#define DANGERENGINE_RESOURCEUTILS_H

#include <stdbool.h>


typedef struct slsAppState slsAppState;
typedef struct slsShader slsShader;


slsShader *sls_load_shader(slsAppState *state,
                           char const *name,
                           char const *fspath,
                           char const *vspath,
                           bool path_is_absolute);

slsShader *sls_get_shader(slsAppState *state, char const *name);



#endif //DANGERENGINE_RESOURCEUTILS_H
