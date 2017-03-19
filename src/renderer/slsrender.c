#include "slsrender.h"
#include "shaderutils.h"

static void scene_setup(slsRendererGL *self);

slsRendererGL *sls_renderer_init(slsRendererGL *self, int width, int height)
{
  sls_renderer_resize(self, width, height);

  scene_setup(self);

  return self;
}

static void scene_setup(slsRendererGL *self)
{
  kmMat4Identity(&self->root_modelview);
  sls_check(sls_mesh_square(&self->mesh), "mesh failed");
  GLuint program = sls_create_program("./resources/shaders/demo.vert",
                                      "./resources/shaders/demo.frag",
                                      "./resources/shaders/uniforms.glsl");
  sls_check(sls_shader_init(&self->shader, program), "shader failed");

  sls_mesh_bind(&self->mesh, &self->shader);

  return;
  error:
  exit(EXIT_FAILURE);
}


slsRendererGL *sls_renderer_dtor(slsRendererGL *self)
{
  sls_mesh_dtor(&self->mesh);
  sls_shader_dtor(&self->shader);
  return self;
}

void sls_renderer_display(slsRendererGL *self)
{
  sls_shader_use(&self->shader);
  sls_mesh_draw(&self->mesh);
}


void sls_renderer_resize(slsRendererGL *self, int width, int height)
{
  self->width = width;
  self->height = height;
  float aspect = width / (float) height;
  kmMat4PerspectiveProjection(&self->projection, 45.f, aspect, 0.01f, 1000.f);

}
