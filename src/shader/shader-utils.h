//
// Created by Steven on 6/17/15.
//

#ifndef DANGERENGINE_SHADER_UTILS_H
#define DANGERENGINE_SHADER_UTILS_H

#include "../sls-gl.h"
#include "../slsutils.h"

char *sls_read_file(char const *path);
GLuint sls_compile_shader_from_file(char const *path, GLenum shader_type, slsBool *res);
GLuint sls_compile_shader(char const *src, GLenum type, slsBool *res);

slsBool sls_check_shader(GLuint shader);
slsBool sls_check_program(GLuint program);


#endif //DANGERENGINE_SHADER_UTILS_H
