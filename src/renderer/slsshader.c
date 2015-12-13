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
 *    and/or other materials provided with the distribution.
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

#include <apr_pools.h>
#include <sls-gl.h>
#include <assert.h>
#include "slsshader.h"

static slsShader shader_proto = {
    .dtor = sls_shader_dtor, .init = sls_shader_init,
};
slsShader const *sls_shader_proto() { return &shader_proto; }

slsShader *sls_shader_init(slsShader *self, apr_pool_t *parent_pool,
                           GLuint program) {
  *self = *sls_shader_proto();

  apr_status_t res = apr_pool_create(&self->pool, parent_pool);
  sls_checkmem(res == APR_SUCCESS);

  sls_check(glIsProgram(program), "GLuint %u is not a program", program);

  self->attr_table = apr_hash_make(self->pool);
  sls_checkmem(self->attr_table);
  self->unif_table = apr_hash_make(self->pool);
  sls_checkmem(self->unif_table);

  if (self->unif_table && self->attr_table) {
    self->program = program;
    sls_shader_bind_attrs(self);
    sls_shader_bind_unifs(self);
  }

  self->owns_program = false;

  return self;
error:
  if (self) {
    return sls_shader_dtor(self);
  }
}

slsShader *sls_shader_dtor(slsShader *self) {

  if (self->pool) {
    apr_pool_destroy(self->pool);
    self->pool = NULL;
  }

  if (self->owns_program) {
    glDeleteProgram(self->program);
  }

  return self;
}

void sls_shader_bind_unifs(slsShader *self) {
  typeof(self->uniforms) *u = &self->uniforms;

  struct unif_pair {
    GLuint *loc;
    char const *name;
  };

  struct unif_pair pairs[] = {
      {&u->time, "time"},
      {&u->model_view, "model_view"},
      {&u->projection, "projection"},
      {&u->normal_mat, "normal_mat"},
      {&u->normal_tex, "normal_tex"},
      {&u->diffuse_tex, "diffuse_tex"},
      {&u->specular_tex, "specular_tex"},
      {&u->lights.diffuse_products, "lights.diffuse_products"},

      {&u->lights.ambient_products, "lights.ambient_products"},

      {&u->lights.specular_products, "lights.specular_products"},

      {&u->lights.light_positions, "lights.light_positions"},
      {&u->lights.light_modelview, "lights.light_modelview"},

      {&u->lights.active_lights, "lights.n_lights"}

  };

  for (int i = 0; i < sizeof(pairs) / sizeof(struct unif_pair); ++i) {
    struct unif_pair *itor = pairs + i;

    if (itor->loc) {
      bool res = false;
      *itor->loc = sls_shader_get_unif(self, itor->name, &res);
    }
  }
}

void sls_shader_bind_attrs(slsShader *self) {
  typedef struct {
    GLuint *a;
    char *name;
  } AttrPair;
  typeof(self->attributes) *a = &self->attributes;
  AttrPair attrs[] = {(AttrPair){&a->position, "position"},
                      (AttrPair){&a->normal, "normal"},
                      (AttrPair){&a->uv, "uv"}, (AttrPair){&a->color, "color"}};

  size_t len = sizeof(attrs) / sizeof(AttrPair);
  for (uint32_t i = 0; i < len; ++i) {

    AttrPair *p = attrs + i;
    *p->a = i;
    glBindAttribLocation(self->program, i, attrs[i].name);

    sls_attr_check(p->name, self->program, *p->a);
  }
}

void sls_uniform_check(char const *name, int val) {
  if (val < 0) {
    sls_log_warn("cannot bind uniform <%s>: %i", name, val);
  }
}

void sls_attr_check(char *name, GLuint program, GLuint location) {
  int actual_location = glGetAttribLocation(program, name);
  if (actual_location != location) {
    sls_log_warn("could not bind attribute <%s>: %i", name, actual_location);
  }
}

GLuint sls_shader_get_attr(slsShader *self, char const *variable,
                           bool *result_out) {
  glUseProgram(self->program);
  GLuint r_value = 0;
  bool success = false;

  GLuint *val;

  val = apr_hash_get(self->attr_table, variable, APR_HASH_KEY_STRING);

  if (val) {
    r_value = *val;
    success = true;
  } else {
    int location = glGetAttribLocation(self->program, variable);
    if (location < 0) {
      success = false;
    } else {
      val = apr_pcalloc(self->pool, sizeof(GLuint));
      *val = (GLuint)location;
      apr_hash_set(self->attr_table, variable, APR_HASH_KEY_STRING, val);

      success = true;
    }
  }

  if (result_out) {
    *result_out = success;
  }

  return r_value;
}

GLuint sls_shader_get_unif(slsShader *self, char const *variable,
                           bool *result_out) {
  glUseProgram(self->program);
  GLuint r_value = 0;
  bool success = false;

  GLuint *val;

  val = apr_hash_get(self->unif_table, variable, APR_HASH_KEY_STRING);

  if (val) {
    r_value = *val;
    success = true;
  } else {
    int location = glGetUniformLocation(self->program, variable);
    if (location < 0) {
      sls_uniform_check(variable, location);
      success = false;
    } else {
      val = apr_pcalloc(self->pool, sizeof(GLuint));
      *val = (GLuint)location;
      apr_hash_set(self->unif_table, variable, APR_HASH_KEY_STRING, val);

      r_value = *val;

      success = true;
    }
  }

  if (result_out) {
    *result_out = success;
  }

  return r_value;
}
