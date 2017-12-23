#include "slsrender.h"

static void scene_setup(slsRendererGL* self);

slsRendererGL* sls_renderer_init(slsRendererGL* self, int width, int height)
{
  // setup opengl pipeline

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_POINT_SIZE);

  glEnable(GL_POINT_SPRITE_ARB);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  scene_setup(self);
  sls_renderer_resize(self, width, height);

  return self;
}

static void scene_setup(slsRendererGL* self)
{
  kmMat4Translation(&self->root_modelview, 0.f, 0.f, -5.f);

}

slsRendererGL* sls_renderer_dtor(slsRendererGL* self)
{
  return self;
}

void sls_renderer_resize(slsRendererGL* self, int width, int height)
{
  self->width = width;
  self->height = height;
  float aspect = width / (float)height;
  kmMat4PerspectiveProjection(&self->projection, 45.f, aspect, 0.01f, 1000.f);
}

