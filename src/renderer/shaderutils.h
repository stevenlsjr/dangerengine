/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#ifndef _SLS_SHADERUTILS_H_
#define _SLS_SHADERUTILS_H_

#include "../sls-gl.h"

char* sls_file_read(const char* filename);

/** glsl debug log **/
void sls_print_log(GLuint object);

GLuint sls_create_shader(const char *filename, GLenum type);

GLuint sls_create_program(const char *vertexfile, const char *fragmentfile);

GLuint sls_create_gs_program(const char *vertexfile, const char *geometryfile,
                             const char *fragmentfile, GLint input,
                             GLint output, GLint vertices);
                            
GLint sls_get_attrib(GLuint program, const char *name);
GLint sls_get_uniform(GLuint program, const char *name);
#endif
