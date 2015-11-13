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

#include <apr_pools.h>
#include <GL/glew.h>
#include <assert.h>
#include "slsshader.h"



slsShader const *sls_shader_proto()
{
  static volatile slsShader proto = {
      .dtor = sls_shader_dtor,
      .init = sls_shader_init,
  };

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"
  return &proto;
#pragma clang diagnostic pop
}


slsShader *sls_shader_init(slsShader *self, apr_pool_t *parent_pool, GLuint program)
{
  *self = *sls_shader_proto();

  apr_status_t res = apr_pool_create(&self->pool, parent_pool);
  sls_checkmem(res == APR_SUCCESS);

  sls_check(glIsProgram(program), "GLuint %u is not a program", program);

  self->program = program;
  sls_shader_bind_attrs(self);
  sls_shader_bind_unifs(self);

  self->owns_program = false;

  return self;
  error:
  if (self) {
    return sls_shader_dtor(self);
  }
}


slsShader *sls_shader_dtor(slsShader *self)
{

  if (self->pool) {
    apr_pool_destroy(self->pool);
    self->pool = NULL;
  }

  if (self->owns_program) {
    glDeleteProgram(self->program);
  }

  return self;
}



void sls_shader_bind_unifs(slsShader *self)
{
  typeof(self->uniforms) *u = &self->uniforms;
  struct unif_pair {
    int u;
    char *name;
  };

  struct unif_pair unif;

  glUseProgram(self->program);
  unif.name = "time";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->time = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "model_view";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->model_view = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "projection";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->projection = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "normal_mat";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->normal_mat = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "diffuse_map";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->diffuse_map = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "specular_map";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->specular_map = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "normal_map";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->normal_map = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);

  unif.name = "z_layer";
  unif.u = glGetUniformLocation(self->program, unif.name);
  u->z_layer = (GLuint) unif.u;
  sls_uniform_check(unif.name, unif.u);


  {
    unif.name = "material.diffuse_color";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->material.diffuse_color = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "ambient_color";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->material.ambient_color = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "specular_color";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->material.specular_color = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "shininess";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->material.shininess = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);
  }

  {
    unif.name = "lights.diffuse_products";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->lights.diffuse_products = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "lights.ambient_products";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->lights.ambient_products = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "lights.specular_products";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->lights.specular_products = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "lights.light_positions";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->lights.light_positions = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);

    unif.name = "lights.active_lights";
    unif.u = glGetUniformLocation(self->program, unif.name);
    u->lights.active_lights = (GLuint) unif.u;
    sls_uniform_check(unif.name, unif.u);
  }

}

void sls_shader_bind_attrs(slsShader *self)
{
  typedef struct {
    GLuint *a;
    char *name;
  } AttrPair;
  typeof(self->attributes) *a = &self->attributes;
  AttrPair attrs[] = {(AttrPair) {&a->position, "position"},
                      (AttrPair) {&a->normal, "normal"},
                      (AttrPair) {&a->uv, "uv"},
                      (AttrPair) {&a->color, "color"}};


  size_t len = sizeof(attrs) / sizeof(AttrPair);
  for (uint32_t i = 0; i < len; ++i) {


    AttrPair *p = attrs + i;
    *p->a = i;
    glBindAttribLocation(self->program, i, attrs[i].name);

    sls_attr_check(p->name, self->program, *p->a);

  }
}



void sls_uniform_check(char const *name, int val)
{
  if (val < 0) {
    sls_log_warn("cannot bind uniform <%s>: %i", name, val);
  }
}


void sls_attr_check(char *name, GLuint program, GLuint location)
{
  int actual_location = glGetAttribLocation(program, name);
  if (actual_location != location) {
    sls_log_warn("could not bind attribute <%s>: %i",
                 name,
                 actual_location);
  }
}
