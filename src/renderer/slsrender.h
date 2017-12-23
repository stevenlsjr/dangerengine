/**
 * @file slsrender.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
**/

#ifndef SLS_RENDERER_H
#define SLS_RENDERER_H

#include "slsmesh.h"
#include <kazmath/kazmath.h>
#include <slsmacros.h>
#include <slscontext.h>

SLS_BEGIN_CDECLS

typedef struct slsRendererGL slsRendererGL;
typedef struct slsRenderList slsRenderList;
typedef struct slsAppState slsAppState;

/**
 * @brief opengl renderer instance
 */
struct slsRendererGL {

  slsShader mesh_shader;
  slsMesh mesh;
  // dummy scene description
  kmMat4 root_modelview;
  kmMat4 projection;

  int width, height;
};

/**
 * @brief Enum flagging projection type
 * used for camera definitions
 */
typedef enum slsProjectionType {
  SLS_PROJECTION_ORTHO,
  SLS_PROJECTION_PERSPECTIVE,
  SLS_PROJECTION_COUNT
} slsProjectionType;

/**
 * Initialize renderer
 * @param self
 * @param width
 * @param height
 * @return
 */
slsRendererGL *sls_renderer_init(slsRendererGL *self, int width, int height)
SLS_NONNULL(1);

/**
 * Destroys renderer
 * @param self
 * @return
 */
slsRendererGL *sls_renderer_dtor(slsRendererGL *self) SLS_NONNULL(1);

static void sls_renderer_clear(slsRendererGL *self){
  glClear(GL_COLOR_BUFFER_BIT);
}
static void sls_renderer_swap(slsRendererGL *self, slsContext *ctx){
  SDL_GL_SwapWindow(ctx->window);
}

void sls_renderer_resize(slsRendererGL *self, int width, int height)
SLS_NONNULL(1);

SLS_END_CDECLS

#endif /* SLS_RENDERER_H */
