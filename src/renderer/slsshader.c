//
// Created by Steven on 6/23/15.
//

/**
 * @file slsshader.c
 * @brief
 *
**/

#include "slsshader.h"
#include "shaderutils.h"
#include <sls-gl.h>

slsShader *
sls_shader_from_sources(slsShader *self, char const *vs_source, char const *fs_source,
                        char const *uniforms)
{
  if (!uniforms) {
    uniforms = SLS_DEFAULT_UNIFORMS;
  }
  GLuint program = sls_create_program(vs_source, fs_source, uniforms);
  self = sls_shader_init(self, program);
  return self;
}

slsShader* sls_shader_init(slsShader* self, GLuint program)
{
  *self = (slsShader) {};

  sls_check(glIsProgram(program), "GLuint %u is not a program", program);

  self->program = program;

  // bind hardcoded active_shader attributes and uniforms
  glBindAttribLocation(program, SLS_ATTRIB_POSITION, "position");
  glBindAttribLocation(program, SLS_ATTRIB_NORMAL, "normal");
  glBindAttribLocation(program, SLS_ATTRIB_UV, "uv");
  glBindAttribLocation(program, SLS_ATTRIB_COLOR, "color");

  return self;
error:
  if (self) {
    return sls_shader_dtor(self);
  }

  return NULL;
}

slsShader* sls_shader_dtor(slsShader* self)
{
  glDeleteProgram(self->program);

  return self;
}

void sls_shader_use(slsShader* self)
{
  GLuint prg = self ? self->program : 0;
  glUseProgram(prg);
}

void sls_shader_bind_vec3(slsShader* self, GLuint location, kmVec3 vec)
{
  sls_shader_bind_vec3v(self, location, &vec, 1);
}

void sls_shader_bind_vec4(slsShader* self, GLuint location, kmVec4 vec)
{
  sls_shader_bind_vec4v(self, location, &vec, 1);
}

void sls_shader_bind_mat4(slsShader* self,
                          GLuint location,
                          kmMat4 const* m,
                          bool transpose)
{
  sls_shader_bind_mat4v(self, location, m, 1, transpose);
}

void sls_shader_bind_mat3(slsShader* self,
                          GLuint location,
                          kmMat3 const* m,
                          bool transpose)
{
  sls_shader_bind_mat3v(self, location, m, 1, transpose);
}

void sls_shader_bind_mat4v(slsShader* self,
                           GLuint location,
                           kmMat4 const* m,
                           size_t count,
                           bool transpose)
{
  sls_shader_use(self);
  glUniformMatrix4fv(location, (GLsizei)count, (GLboolean)transpose, m->mat);
}

void sls_shader_bind_mat3v(slsShader* self,
                           GLuint location,
                           kmMat3 const* m,
                           size_t count,
                           bool transpose)
{
  sls_shader_use(self);
  glUniformMatrix3fv(location, (GLsizei)count, (GLboolean)transpose, m->mat);
}

void sls_shader_bind_vec3v(slsShader* self,
                           GLuint location,
                           kmVec3 const* vec,
                           size_t count)
{
  sls_shader_use(self);
  glUniform3fv(location, (GLsizei)count, &vec->x);
}

void sls_shader_bind_vec4v(slsShader* self,
                           GLuint location,
                           kmVec4 const* vec,
                           size_t count)
{
  sls_shader_use(self);
  glUniform4fv(location, (GLsizei)count, &vec->x);
}

