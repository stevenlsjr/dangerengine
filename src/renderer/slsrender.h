
#ifndef SLS_RENDERER_H
#define SLS_RENDERER_H

#include <slsmacros.h>
#include <kazmath/kazmath.h>
#include "slsmesh.h"

SLS_BEGIN_CDECLS

typedef struct slsRendererGL slsRendererGL;


struct slsRendererGL {
  // dummy scene description
  // TODO: add scene drawing api
  kmMat4 root_modelview;
  kmMat4 projection;

  slsMesh mesh;
  slsShader shader;
  int width, height;
};

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
slsRendererGL *sls_renderer_dtor(slsRendererGL *self); SLS_NONNULL(1)

/**
 * draws rendered scene via OpenGL
 * @param self
 */
void sls_renderer_display(slsRendererGL *self) SLS_NONNULL(1);

void sls_renderer_resize(slsRendererGL *self, int width, int height) SLS_NONNULL(1);

SLS_END_CDECLS

#endif /* SLS_RENDERER_H */
