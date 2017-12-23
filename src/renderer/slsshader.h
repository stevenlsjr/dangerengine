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
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#ifndef DANGERENGINE_SLSSHADER_H
#define DANGERENGINE_SLSSHADER_H

#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>
#include <sls-gl.h>
#include <slsutils.h>

typedef struct slsShader slsShader;
typedef struct slsUniformLocations slsUniformLocations;
typedef struct slsAttrLocations slsAttrLocations;

enum slsDefaultAttribLocations {
  SLS_ATTRIB_POSITION = 0,
  SLS_ATTRIB_NORMAL = 1,
  SLS_ATTRIB_UV = 2,
  SLS_ATTRIB_COLOR = 3,
  SLS_ATTRIB_LOCATIONS_LAST
};

typedef enum slsDefaultAttribLocations slsDefaultAttribLocations;


/**
 * @brief struct storing attribute locations for default shaders
 */
struct slsAttrLocations {
  GLuint position, normal, uv, color;
};

/**
 * @brief: struct storing uniform locations for most shader programs
 * @details uniform header file at /resources/shaders/uniforms.glsl
 */
struct slsUniformLocations {
  GLuint model_view, inv_model_view, normal_mat, time, diffuse_tex,
      specular_tex, normal_tex;

  struct {
    GLuint specular_color, diffuse_color, ambient_color, shininess;
  } material;

  struct {
    GLuint n_lights, ambient_products, diffuse_products, specular_products,
        light_positions, light_modelview;
  } lights;
};

struct slsShader {

  GLuint program;

  slsUniformLocations uniforms;
  void *data;
};



//
// Attribute/Uniform location tables
//

slsShader *sls_shader_from_sources(slsShader *self, char const *vs_source, char const *fs_source,
                                   char const *uniforms);

slsShader *sls_shader_init(slsShader *self, GLuint program) SLS_NONNULL(1);

slsShader *sls_shader_dtor(slsShader *self) SLS_NONNULL(1);

void sls_setup_attribs(slsShader *self);

void sls_shader_use(slsShader *self_opt);

void sls_shader_bind_vec3(slsShader *self, GLuint location, kmVec3 vec)
SLS_NONNULL(1);

void sls_shader_bind_vec4(slsShader *self, GLuint location, kmVec4 vec)
SLS_NONNULL(1);

void sls_shader_bind_vec3v(slsShader *self,
                           GLuint location,
                           kmVec3 const *vec,
                           size_t count) SLS_NONNULL(1);

void sls_shader_bind_vec4v(slsShader *self,
                           GLuint location,
                           kmVec4 const *vec,
                           size_t count) SLS_NONNULL(1);

void sls_shader_bind_mat4(slsShader *self,
                          GLuint location,
                          kmMat4 const *m,
                          bool transpose) SLS_NONNULL(1, 3);

void sls_shader_bind_mat3(slsShader *self,
                          GLuint location,
                          kmMat3 const *m,
                          bool transpose) SLS_NONNULL(1, 3);

void sls_shader_bind_mat4v(slsShader *self,
                           GLuint location,
                           kmMat4 const *m,
                           size_t count,
                           bool transpose) SLS_NONNULL(1, 3);

void sls_shader_bind_mat3v(slsShader *self,
                           GLuint location,
                           kmMat3 const *m,
                           size_t count,
                           bool transpose) SLS_NONNULL(1, 3);

extern char const *SLS_DEFAULT_VS;
extern char const *SLS_DEFAULT_FS;
extern char const *SLS_DEFAULT_UNIFORMS;

#endif // DANGERENGINE_SLSSHADER_H
