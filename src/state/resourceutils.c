/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/13/15, Steven
 * 
 **/
#include <MacTypes.h>
#include <renderer/slsshader.h>
#include <renderer/shaderutils.h>
#include "resourceutils.h"
#include "slsAppState.h"
#include "slsBehavior.h"

slsTexture *sls_load_texture(slsAppState *state,
                             char const *name,
                             char const *diffuse_map,
                             char const *spec_map,
                             char const *norm_map,
                             bool path_is_absolute)
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
  return NULL;
}

slsShader *sls_load_shader(slsAppState *state,
                           char const *name,
                           char const *fspath,
                           char const *vspath,
                           bool path_is_absolute)
{

  slsShader *shader = NULL;
  GLuint program = 0;
  sls_check(!apr_hash_get(state->shaders, name, APR_HASH_KEY_STRING),
            "shader named %s already exists", name);


  shader = apr_pcalloc(state->pool, sizeof(slsShader));
  sls_checkmem(shader);

  // TODO: add parameter for loading uniform decl path
  program = sls_create_program(vspath, fspath, "resources/shaders/uniforms.glsl");

  shader = sls_shader_init(shader, state->pool, program);
  sls_checkmem(shader);

  return shader;

error:
  if (shader) {
    sls_shader_dtor(shader);
  }
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
