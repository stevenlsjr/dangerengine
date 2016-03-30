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

slsShader const *sls_shader_proto() { return &shader_proto; }

slsShader *sls_shader_init(slsShader *self, GLuint program)
{
  *self = *sls_shader_proto();


  sls_check(glIsProgram(program), "GLuint %u is not a program", program);

  sls_checkmem(sls_locationtable_init(&self->attr_table));
  sls_checkmem(sls_locationtable_init(&self->unif_table));


  self->program = program;
  sls_shader_bind_attrs(self);
  sls_shader_bind_unifs(self);

  self->owns_program = false;

  return self;
  error:
  if (self) {
    return sls_shader_dtor(self);
  }

  return NULL;
}

slsShader *sls_shader_dtor(slsShader *self)
{
  sls_locationtable_dtor(&self->attr_table);
  sls_locationtable_dtor(&self->unif_table);



  if (self->owns_program) {
    glDeleteProgram(self->program);
  }

  return self;
}

void sls_shader_bind_unifs(slsShader *self)
{
  //typeof(self->uniforms) *u = &self->uniforms;

  char const *unifs[] = {
      "time",
      "model_view",
      "projection",
      "normal_mat",
      "normal_tex",
      "diffuse_tex",
      "specular_tex",
      "lights.diffuse_products",
      "lights.ambient_products",
      "lights.specular_products",
      "lights.light_positions",
      "lights.light_modelview",
      "lights.n_lights"

  };

  for (int i = 0; i < sizeof(unifs) / sizeof(char *); ++i) {

    char const *name = unifs[i];


    GLint loc = glGetUniformLocation(self->program, name);


    sls_locationtable_set(&self->unif_table, name, (GLuint) (loc >= 0 ? loc : 0));

  }
}


void sls_shader_bind_attrs(slsShader *self)
{

  char const *attrs[] = {"position",
                         "normal",
                         "uv",
                         "color"};

  size_t len = sizeof(attrs) / sizeof(char const *);
  for (uint32_t i = 0; i < len; ++i) {

    char const *name = attrs[i];

    glBindAttribLocation(self->program, i, name);
    sls_locationtable_set(&self->attr_table, name, i);

    sls_attr_check(name, self->program, i);
  }
}

void sls_uniform_check(char const *name, int val)
{
  if (val < 0) {
    sls_log_warn("cannot bind uniform <%s>: %i", name, val);
  }
}

void sls_attr_check(char const *name, GLuint program, GLuint location)
{
  int actual_location = glGetAttribLocation(program, name);
  if (actual_location != location) {
    sls_log_warn("could not bind attribute <%s>: %i", name, actual_location);
  }
}

GLuint sls_shader_get_attr(slsShader *self, char const *variable, bool *result_out)
{
  return sls_locationtable_get_val(&self->attr_table, variable);
}

GLuint sls_shader_get_unif(slsShader *self, char const *variable, bool *result_out)
{
  return sls_locationtable_get_val(&self->unif_table, variable);
}
