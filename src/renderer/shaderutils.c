/**
 * From the OpenGL Programming wikibook:
 * http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */

#include "../sls-gl.h"
#include "../slsutils.h"
#include <stdio.h>
#include <stdlib.h>

#include "shaderutils.h"

int sls_get_glversion();

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char* sls_file_read(const char* filename)
{
  FILE* file = fopen(filename, "rb");
  if (file == NULL)
    return NULL;

  int file_size = BUFSIZ;
  char* file_str = (char*)malloc((size_t)file_size);
  int nb_read_total = 0;

  while (!feof(file) && !ferror(file)) {
    if (nb_read_total + BUFSIZ > file_size) {
      if (file_size > 10 * 1024 * 1024)
        break;
      file_size = file_size * 2;
      file_str = (char*)realloc(file_str, (size_t)file_size);
    }
    char* p_res = file_str + nb_read_total;
    nb_read_total += fread(p_res, 1, BUFSIZ, file);
  }

  fclose(file);
  file_str = (char*)realloc(file_str, nb_read_total + 1);
  file_str[nb_read_total] = '\0';
  return file_str;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void _sls_print_log(GLuint object,
                    char const* shader_file,
                    char const* file,
                    char const* func,
                    long line)
{
  if (file && func) {
    fprintf(stderr, "Shader Error: %s, %s: %li:\n", file, func, line);
  }
  GLint log_length = 0;
  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else if (glIsProgram(object)) {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else {
    fprintf(stderr, "Not a active_shader or a program\n");
    return;
  }

  char* log = (char*)malloc((size_t)log_length);

  if (glIsShader(object)) {
    glGetShaderInfoLog(object, log_length, NULL, log);
  } else if (glIsProgram(object)) {
    glGetProgramInfoLog(object, log_length, NULL, log);
  }

  fprintf(stderr, "%s\n", log);
  free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint sls_create_shader(const char* filename,
                         char const* uniform_file_name,
                         GLenum type)
{
  GLchar const* modern_preamble = "#version 330\n#define SLS_MODERN_OPENGL 1\n";
  GLchar const* legacy_preamble = "#version 130\n";
  GLchar const* gles_preamble = "#version 100\n";

  GLchar const* preamble;

#ifndef SLS_GLES
  int v = sls_get_glversion();
  if (v >= 330) {
    preamble = modern_preamble;
  } else {
    preamble = legacy_preamble;
  }

#else
  preamble = gles_preamble;
#endif

  GLchar* source = sls_file_read(filename);
  char* uniform_src = sls_file_read(uniform_file_name);

  if (!uniform_src) {
    sls_log_err("Error opening %s: ", uniform_file_name);
    perror("");
    return 0;
  }

  if (!source) {
    sls_log_err("Error opening %s: ", filename);
    perror("");
    return 0;
  }
  GLuint res = glCreateShader(type);

  char const* sources[] = { preamble, uniform_src, source };
  const size_t n_sources = sizeof(sources) / sizeof(char*);

  glShaderSource(res, n_sources, sources, NULL);

  free(source);
  free(uniform_src);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

  if (compile_ok == GL_FALSE) {
    fprintf(stderr, "%s:", filename);
    sls_print_log(res, filename);
    glDeleteShader(res);

    return 0;
  }

  return res;
}

GLuint sls_create_program(const char* vertexfile,
                          const char* fragmentfile,
                          char const* uniform_definitions)
{
  GLuint program = glCreateProgram(), vs = 0, fs = 0;

  sls_checkmem(vertexfile && fragmentfile && uniform_definitions);

  vs = sls_create_shader(vertexfile, uniform_definitions, GL_VERTEX_SHADER);
  if (vs == 0) {
    sls_log_err("vertex active_shader at path %s is invalid", vertexfile);
    sls_checkmem(vs != 0);
    return 0;
  }
  glAttachShader(program, vs);

  fs = sls_create_shader(fragmentfile, uniform_definitions, GL_FRAGMENT_SHADER);
  if (fs == 0) {
    sls_log_err("fragment active_shader at path %s is invalid", fragmentfile);
    glDeleteShader(vs);
    sls_checkmem(fs != 0);
    return 0;
  }
  glAttachShader(program, fs);

  //

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    sls_print_log(program, "program");
    glDeleteProgram(program);
    return 0;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;

error:
  glDeleteProgram(program);
  return 0;
}

#ifdef GL_GEOMETRY_SHADER

GLuint sls_create_gs_program(const char* vertexfile,
                             const char* geometryfile,
                             const char* fragmentfile,
                             char const* uniform_definitions,
                             GLint input,
                             GLint output,
                             GLint vertices)
{
  GLuint program = glCreateProgram();
  GLuint shader;

  if (vertexfile) {
    shader =
      sls_create_shader(vertexfile, uniform_definitions, GL_VERTEX_SHADER);
    if (!shader)
      return 0;
    glAttachShader(program, shader);
  }

  if (geometryfile) {
    shader =
      sls_create_shader(geometryfile, uniform_definitions, GL_GEOMETRY_SHADER);
    if (!shader)
      return 0;
    glAttachShader(program, shader);

    glProgramParameteri(program, GL_GEOMETRY_INPUT_TYPE, input);
    glProgramParameteri(program, GL_GEOMETRY_OUTPUT_TYPE, output);
    glProgramParameteri(program, GL_GEOMETRY_VERTICES_OUT, vertices);
  }

  if (fragmentfile) {
    shader =
      sls_create_shader(fragmentfile, uniform_definitions, GL_FRAGMENT_SHADER);
    if (shader == 0) {
      return 0;
    }
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    sls_print_log(program, "program");
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

#else
GLuint create_gs_program(const char* vertexfile,
                         const char* geometryfile,
                         const char* fragmentfile,
                         GLint input,
                         GLint output,
                         GLint _vertices)
{
  fprintf(stderr, "Missing support for geometry shaders.\n");
  return 0;
}
#endif

GLint sls_get_attrib(GLuint program, const char* name)
{
  GLint attribute = glGetAttribLocation(program, name);
  if (attribute == -1)
    fprintf(stderr, "Could not bind attribute %s\n", name);
  return attribute;
}

GLint sls_get_uniform(GLuint program, const char* name)
{
  GLint uniform = glGetUniformLocation(program, name);
  if (uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", name);
  return uniform;
}
