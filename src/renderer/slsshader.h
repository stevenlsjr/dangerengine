//
// Created by Steven on 6/23/15.
//

/**
 * @file slsshader.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea.
**/

#ifndef DANGERENGINE_SLSSHADER_H
#define DANGERENGINE_SLSSHADER_H

#include <sls-gl.h>
#include <slsutils.h>
#include <apr_pools.h>
#include <apr_hash.h>
#include <data-types/hashtable.h>

typedef struct slsShader slsShader;


struct slsShader {
  slsShader *(*init)(slsShader *self,
                     apr_pool_t *parent_pool,
                     GLuint program) SLS_NONNULL(1, 2);

  slsShader *(*dtor)(slsShader *self) SLS_NONNULL(1);

  struct {
    GLuint model_view;
    GLuint normal_mat;
    GLuint projection;

    GLuint time;

    GLuint diffuse_tex;
    GLuint specular_tex;
    GLuint normal_tex;

    GLuint z_layer;

    struct {
      GLuint diffuse_color;
      GLuint ambient_color;
      GLuint specular_color;

      GLuint shininess;
    } material;
    struct {

      GLuint n_lights;
      GLuint ambient_products;
      GLuint diffuse_products;
      GLuint specular_products;
      GLuint light_positions;
      GLuint light_modelview;

      GLuint active_lights;
    } lights;

  } uniforms;

  struct {
    GLuint position;
    GLuint normal;
    GLuint uv;
    GLuint color;
    GLuint z_layer;
  } attributes;

#if 0
  apr_hash_t *attr_table;
  apr_hash_t *unif_table;
#else

  slsHashTable *attr_table;
  slsHashTable *unif_table;
#endif

  GLuint program;
  bool owns_program;

  apr_pool_t *pool;
  void *data;
};

slsShader const *sls_shader_proto();

slsShader *sls_shader_init(slsShader *self,
                           apr_pool_t *parent_pool,
                           GLuint program) SLS_NONNULL(1, 2);

slsShader *sls_shader_dtor(slsShader *self) SLS_NONNULL(1);

void sls_shader_bind_attrs(slsShader *self) SLS_NONNULL(1);

void sls_shader_bind_unifs(slsShader *self) SLS_NONNULL(1);

GLuint sls_shader_get_attr(slsShader *self, char const *variable, bool *result_out);
GLuint sls_shader_get_unif(slsShader *self, char const *variable, bool *result_out);


void sls_uniform_check(char const *name, int val);
void sls_attr_check(char *name, GLuint program, GLuint location);



#endif //DANGERENGINE_SLSSHADER_H
