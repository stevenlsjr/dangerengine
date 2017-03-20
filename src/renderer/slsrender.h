/**
 * @file slsrender.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
**/


#ifndef SLS_RENDERER_H
#define SLS_RENDERER_H

#include <slsmacros.h>
#include <kazmath/kazmath.h>
#include "slsmesh.h"

SLS_BEGIN_CDECLS

typedef struct slsRendererGL slsRendererGL;
typedef struct slsRenderList slsRenderList;

/**
 * @brief opengl renderer instance
 */
struct slsRendererGL {
  // dummy scene description
  kmMat4 root_modelview;
  kmMat4 projection;

  int width, height;

  // currently rendering one mesh at a time
  slsShader *active_shader;
  slsMesh *active_mesh;

  slsRenderList *render_list;
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
slsRendererGL *sls_renderer_dtor(slsRendererGL *self);

SLS_NONNULL(1)

/**
 * draws rendered scene via OpenGL
 * @param self
 */
void sls_renderer_display(slsRendererGL *self) SLS_NONNULL(1);

void sls_renderer_resize(slsRendererGL *self, int width, int height)
SLS_NONNULL(1);

void sls_renderer_draw_mesh(slsRendererGL *self, slsMesh *mesh, slsShader *shader,
                            kmMat4 *model_view);


typedef struct slsRenderEntry slsRenderEntry;
struct slsRenderEntry {
  kmMat4 model_view;
  slsMesh *mesh;
};

struct slsRenderList {
  size_t alloc_size;
  size_t count;
  slsRenderEntry *meshes;
};


slsRenderList *sls_renderlist_new(size_t alloc_size);

void sls_renderlist_delete(slsRenderList *self);

void
sls_renderlist_push(slsRenderList *self, slsMesh *mesh, kmMat4 *mat) SLS_NONNULL(1, 2, 3);

SLS_END_CDECLS

#endif /* SLS_RENDERER_H */
