//
// Created by Steven on 6/23/15.
//

/**
 * @file slsshader.c
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

#include <sls-gl.h>
#include <assert.h>
#include "slsshader.h"

static slsShader shader_proto = {
    .dtor = sls_shader_dtor, .init = sls_shader_init,
};

// TODO: bind hardcoded shader locations
static void shader_attrs(slsAttrLocations *attrs) {}
static void shader_unifs(slsUniformLocations *unifs) {}

slsShader const *sls_shader_proto() { return &shader_proto; }

slsShader *sls_shader_init(slsShader *self, GLuint program) {
  *self = *sls_shader_proto();

  sls_check(glIsProgram(program), "GLuint %u is not a program", program);
  shader_attrs(&self->attributes);
  shader_unifs(&self->uniforms);

  self->program = program;

  self->owns_program = false;

  return self;
error:
  if (self) {
    return sls_shader_dtor(self);
  }

  return NULL;
}

slsShader *sls_shader_dtor(slsShader *self) {
  if (self->owns_program) {
    glDeleteProgram(self->program);
  }

  return self;
}

void sls_shader_use(slsShader *self) {
  GLuint prg = self ? self->program : 0;
  glUseProgram(prg);
}

void sls_shader_bind_vec3(slsShader *self, GLuint location, kmVec3 vec) {
  sls_shader_bind_vec3v(self, location, &vec, 1);
}

void sls_shader_bind_vec4(slsShader *self, GLuint location, kmVec4 vec) {
  sls_shader_bind_vec4v(self, location, &vec, 1);
}

void sls_shader_bind_mat4(slsShader *self, GLuint location, kmMat4 const *m,
                          bool transpose) {
  sls_shader_bind_mat4v(self, location, m, 1, transpose);
}

void sls_shader_bind_mat3(slsShader *self, GLuint location, kmMat3 const *m,
                          bool transpose) {
  sls_shader_bind_mat3v(self, location, m, 1, transpose);
}

void sls_shader_bind_mat4v(slsShader *self, GLuint location, kmMat4 const *m,
                           size_t count, bool transpose) {
  sls_shader_use(self);
  glUniformMatrix4fv(location, (GLsizei)count, (GLboolean)transpose, m->mat);
}

void sls_shader_bind_mat3v(slsShader *self, GLuint location, kmMat3 const *m,
                           size_t count, bool transpose) {
  sls_shader_use(self);
  glUniformMatrix3fv(location, (GLsizei)count, (GLboolean)transpose, m->mat);
}

void sls_shader_bind_vec3v(slsShader *self, GLuint location, kmVec3 const *vec,
                           size_t count) {
  sls_shader_use(self);
  glUniform3fv(location, (GLsizei)count, &vec->x);
}

void sls_shader_bind_vec4v(slsShader *self, GLuint location, kmVec4 const *vec,
                           size_t count) {
  sls_shader_use(self);
  glUniform4fv(location, (GLsizei)count, &vec->x);
}
