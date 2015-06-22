//
// Created by Steven on 5/3/15.
//

#ifndef DANGERENGINE_SLS_GL_H
#define DANGERENGINE_SLS_GL_H

/**
 * use gles directly if
 */
#ifdef __EMSCRIPTEN__
#   define SLS_NOGLEW
#   define GLFW_INCLUDE_NONE
#   include <GLES2/gl2.h>
#else
#   define GLFW_INCLUDE_NONE
#   include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>


#endif //DANGERENGINE_SLS_GL_H
