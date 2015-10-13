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
typedef struct slsTexture slsTexture;
typedef struct slsShader slsShader;

slsTexture *sls_load_image(slsAppState *state,
                           char const *name,
                           char const *diffuse_map,
                           char const *spec_map,
                           char const *norm_map);

slsShader *sls_load_shader(slsAppState *state,
                           char const *name,
                           char const *fspath,
                           char const *vspath);

slsShader *sls_get_shader(slsAppState *state, char const *name);
slsTexture *sls_get_texture(slsAppState *state, char const *name);



#endif //DANGERENGINE_RESOURCEUTILS_H
