/**
 * @file ${FILE}
 * @brief Provides appropriate definitions for the
 * nuklear header library as well as bindings
 * to the SDL/OpenGL platform
 * @license ${LICENSE}
 * Copyright (c) 4/20/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLS_NUKLEAR_H
#define DANGERENGINE_SLS_NUKLEAR_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include "nuklear.h"
#include <sls-gl.h>


typedef struct slsNkDevice slsNkDevice;

struct slsNkDevice {
  struct nk_buffer cmds;
  struct nk_draw_null_texture null_tex;
  GLuint vbo, vao, ibo;
  GLuint program;
  GLuint vs, fs;
  struct {
    GLuint pos, uv, color;
  } attributes;
  struct {
    GLuint texture, projection;
  } uniforms;
  GLuint font_tex;
};

static slsNkDevice *sls_nkdevice_init(slsNkDevice *self);
static slsNkDevice *sls_nkdevice_dtor(slsNkDevice *self);


#endif //DANGERENGINE_SLS_NUKLEAR_H
