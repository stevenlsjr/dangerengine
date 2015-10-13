/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/13/15, Steven
 * 
 **/
#include <MacTypes.h>
#include <renderer/slsshader.h>
#include "resourceutils.h"
#include "slsAppState.h"
#include "slsBehavior.h"

slsTexture *sls_load_image(slsAppState *state,
                           char const *name,
                           char const *diffuse_map,
                           char const *spec_map,
                           char const *norm_map)
{
  slsTexture *tex = NULL;
  tex = sls_texture_new(diffuse_map, spec_map, norm_map);
  sls_checkmem(tex);


  sls_check(apr_hash_get(state->textures, name, APR_HASH_KEY_STRING),
            "texture with key %s already exists", name);

  apr_hash_set(state->textures, name, APR_HASH_KEY_STRING, tex);


  return tex;

  error:
  if (tex) {
    sls_texture_class()->dtor(tex);
  }
}

slsShader *sls_load_shader(slsAppState *state, char const *name, char const *fspath, char const *vspath)
{
  return NULL;
}

slsShader *sls_get_shader(slsAppState *state, char const *name)
{
  return NULL;
}

slsTexture *sls_get_texture(slsAppState *state, char const *name)
{
  return NULL;
}
