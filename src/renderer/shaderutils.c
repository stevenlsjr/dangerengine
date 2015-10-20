/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */

#include <stdio.h>
#include <stdlib.h>
#include "../sls-gl.h"
#include "../slsutils.h"


#include "shaderutils.h"

// TODO(Steve) implement a way to load this as a file
static char const *uniforms =
"/**\n"
" * @file uniforms.glsl\n"
" * @brief dives uniform values for all shaders\n"
" * @license FreeBSD\n"
" **/\n"
"\n"
"uniform mat4 model_view;\n"
"uniform mat4 normal_mat;\n"
"uniform mat4 projection;\n"
"\n"
"uniform float time;\n"
"\n"
"uniform sampler2D diffuse_map;\n"
"uniform sampler2D specular_map;\n"
"uniform sampler2D normal_map;\n"
"uniform int z_layer;\n";


int sls_get_glversion();

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char *sls_file_read(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (file == NULL) return NULL;

  int file_size = BUFSIZ;
  char *file_str = (char *) malloc((size_t) file_size);
  int nb_read_total = 0;

  while (!feof(file) && !ferror(file)) {
    if (nb_read_total + BUFSIZ > file_size) {
      if (file_size > 10 * 1024 * 1024) break;
      file_size = file_size * 2;
      file_str = (char *) realloc(file_str, (size_t) file_size);
    }
    char *p_res = file_str + nb_read_total;
    nb_read_total += fread(p_res, 1, BUFSIZ, file);
  }


  fclose(file);
  file_str = (char *) realloc(file_str, nb_read_total + 1);
  file_str[nb_read_total] = '\0';
  return file_str;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void sls_print_log(GLuint object)
{
  GLint log_length = 0;
  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else if (glIsProgram(object))
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else {
    sls_log_err("Not a shader or a program\n");
    return;
  }

  char *log = (char *) malloc((size_t) log_length);

  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, log_length, NULL, log);

  sls_log_err("%s", log);
  free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint sls_create_shader(const char *filename, GLenum type)
{
  GLchar const *modern_preamble =
      "#version 410\n#define SLS_MODERN_OPENGL 1\n";
  GLchar const *legacy_preamble =
      "#version 130\n";
  GLchar const *gles_preamble =
      "#version 100\n";

  GLchar const *preamble;

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

  GLchar *source = sls_file_read(filename);
  if (!source) {
    sls_log_err("Error opening %s: ", filename);
    perror("");
    return 0;
  }
  GLuint res = glCreateShader(type);

  char const *sources[] = {preamble, uniforms, source};
  const size_t n_sources = sizeof(sources) / sizeof(char *);

  glShaderSource(res, n_sources, sources, NULL);


  free(source);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

  if (compile_ok == GL_FALSE) {
    fprintf(stderr, "%s:", filename);
    sls_print_log(res);


    glDeleteShader(res);

    return 0;
  }

  return res;
}


GLuint sls_create_program(const char *vertexfile,
                          const char *fragmentfile,
                          char const *uniform_definitions)
{
  GLuint
      program = glCreateProgram(),
      vs = 0,
      fs = 0;

  if (vertexfile) {
    vs = sls_create_shader(vertexfile, GL_VERTEX_SHADER);
    if (vs == 0) {
      return 0;
    }
    glAttachShader(program, vs);
  }

  if (fragmentfile) {
    fs = sls_create_shader(fragmentfile, GL_FRAGMENT_SHADER);
    if (fs == 0) {
      return 0;
    }
    glAttachShader(program, fs);
  }

  //

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    sls_print_log(program);
    glDeleteProgram(program);
    return 0;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

#ifdef GL_GEOMETRY_SHADER

GLuint sls_create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile,
                             char const *uniform_definitions, GLint input, GLint output, GLint vertices)
{
  GLuint program = glCreateProgram();
  GLuint shader;

  if (vertexfile) {
    shader = sls_create_shader(vertexfile, GL_VERTEX_SHADER);
    if (!shader)
      return 0;
    glAttachShader(program, shader);
  }

  if (geometryfile) {
    shader = sls_create_shader(geometryfile, GL_GEOMETRY_SHADER);
    if (!shader)
      return 0;
    glAttachShader(program, shader);

    glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, input);
    glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);
    glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
  }

  if (fragmentfile) {
    shader = sls_create_shader(fragmentfile, GL_FRAGMENT_SHADER);
    if (!shader)
      return 0;
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    sls_print_log(program);
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

#else
GLuint create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint _vertices) {
    fprintf(stderr, "Missing support for geometry shaders.\n");
    return 0;
}
#endif

GLint sls_get_attrib(GLuint program, const char *name)
{
  GLint attribute = glGetAttribLocation(program, name);
  if (attribute == -1)
    fprintf(stderr, "Could not bind attribute %s\n", name);
  return attribute;
}

GLint sls_get_uniform(GLuint program, const char *name)
{
  GLint uniform = glGetUniformLocation(program, name);
  if (uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", name);
  return uniform;
}
