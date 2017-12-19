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

/**
 * draws rendered scene via OpenGL
 * @param self
 */
void sls_renderer_display(slsRendererGL *self, slsAppState *state) SLS_NONNULL(1, 2);

void sls_renderer_resize(slsRendererGL *self, int width, int height)
SLS_NONNULL(1);

void sls_renderer_draw_mesh(slsRendererGL *self,
                            slsMesh *mesh,
                            slsShader *shader,
                            kmMat4 *model_view);

SLS_END_CDECLS

#endif /* SLS_RENDERER_H */
