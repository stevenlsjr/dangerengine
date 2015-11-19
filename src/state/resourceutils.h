/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/13/15, Steven
 * 
 **/
#ifndef DANGERENGINE_RESOURCEUTILS_H
#define DANGERENGINE_RESOURCEUTILS_H

typedef struct slsAppState slsAppState;
typedef struct slsMaterial slsMaterial;
typedef struct slsShader slsShader;

slsMaterial *sls_load_texture(slsAppState *state,
                              char const *name,
                              char const *diffuse_map,
                              char const *spec_map,
                              char const *norm_map,
                              bool path_is_absolute);

slsShader *sls_load_shader(slsAppState *state,
                           char const *name,
                           char const *fspath,
                           char const *vspath,
                           bool path_is_absolute);

slsShader *sls_get_shader(slsAppState *state, char const *name);

slsMaterial *sls_get_texture(slsAppState *state, char const *name);


#endif //DANGERENGINE_RESOURCEUTILS_H
