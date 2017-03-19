/**
 * @file
 * @brief
 * @detail
 * From the OpenGL Programming wikibook:
 * http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.\n
 * Contributors: Sylvain Beucler
 * modified by: Steven Shea
 */

#ifndef _SLS_SHADERUTILS_H_
#define _SLS_SHADERUTILS_H_

#include "../sls-gl.h"


char *sls_file_read(const char *filename);

void _sls_print_log(GLuint object, char const *shader_file, char const *file,
                    char const *func, long line) SLS_NONNULL(2);

/**
 * @brief macro to automatically insert debugging info to _sls_print_log
 */
#define sls_print_log(object_GLuint, shader_file_char_const_p)                 \
  do {                                                                         \
    _sls_print_log((object_GLuint), (shader_file_char_const_p), __FILE__,      \
                   __FUNCTION__, __LINE__);                                    \
  } while (0)

GLuint sls_create_shader(const char *filename, char const *uniform_file_name,
                         GLenum type) SLS_NONNULL(1, 2);

GLuint sls_create_program(const char *vertexfile, const char *fragmentfile,
                          char const *uniform_definitions) SLS_NONNULL(1, 2, 3);

GLuint sls_create_gs_program(const char *vertexfile, const char *geometryfile,
                             const char *fragmentfile,
                             char const *uniform_definitions, GLint input,
                             GLint output, GLint vertices);


#endif
