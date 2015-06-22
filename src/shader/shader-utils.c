//
// Created by Steven on 6/17/15.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader-utils.h"
#include "../slsutils.h"
#include "../sls-gl.h"


char *sls_read_file(char const *path)
{
  if (!path) {return NULL;}
  FILE *file = NULL;
  char *res = NULL;
  long len = 0;

  file = fopen(path, "r");

  if (!file) {
    sls_log_err("file %s cannot be opened!", path);
    return NULL;
  }

  long current_pos = ftell(file);
  fseek(file, current_pos, SEEK_END);
  len = ftell(file) - current_pos;
  sls_check(len > 0, "len must be over 0");

  rewind(file);

  res = calloc((size_t)len + 1, sizeof(char));
  sls_checkmem(res);

  size_t i;
  for (i = 0; i<len; ++i) {
    int read = fgetc(file);
    if (read == EOF) {
      break;
    }
    res[i] = (char)read;

  }
  res[i] = '\0';


  return res;

error:
  return NULL;
}


GLuint sls_compile_shader_from_file(char const *path, GLenum shader_type, slsBool *res)
{
  GLuint shader = 0;
  char *src = sls_read_file(path);
  sls_check(src, "could not read file!");

  shader = sls_compile_shader(src, shader_type, res);
  free(src);

  return shader;
  error:
  return 0;
}




GLuint sls_compile_shader(char const *src, GLenum type, slsBool *res)
{
  GLuint shader = glCreateShader(type);
  GLint len = (GLint)strlen(src);
  glShaderSource(shader, 1, &src, &len);
  glCompileShader(shader);

  slsBool status = sls_check_shader(shader);
  if (res) {
    *res = status;
  }

  return shader;
}

slsBool sls_check_shader(GLuint shader)
{
  slsBool res = SLS_FALSE;
  GLint compile_status;
  int log_len;
  char *log;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status) {
    res = SLS_TRUE;
  } else {
    res = SLS_FALSE;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
    log = calloc((size_t)log_len + 1, sizeof(char));
    if (log) {
      glGetShaderInfoLog(shader, log_len, NULL, log);
      sls_log_err("shader log: %s", log);
      free(log);
    }
  }

  return res;
}

slsBool sls_check_program(GLuint program)
{
  slsBool res = SLS_FALSE;
  int log_len = 0;
  char *log = NULL;

  int link_status = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  if (link_status) {
    res = SLS_TRUE;
  } else {
    res = SLS_FALSE;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
    glGetProgramInfoLog(program, log_len, NULL, log);

    log = calloc((size_t)log_len + 1, sizeof(char));
    if (log) {
      sls_log_err("program link log: %s", log);
      free(log);
    }
  }

  return res;
}
