/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include <renderer/slsshader.h>
#include "demo_2.h"
#include "renderer/geometries.h"

typedef struct DemoData {
  slsShader phong_shader;

  slsLightBatch lights;

  slsMesh *cube_mesh;
  slsMesh *rect_mesh;

  kmMat4 camera_view;
  kmMat4 projection;

} DemoData;

static void demo_mv_setup(slsContext *self);

static void demo_scene_setup(slsContext *pContext);


static DemoData data = {};

static slsContext *single_ctx = NULL;

void demo_del_ctx(slsContext *ctx)
{
  assert(ctx == single_ctx);

  free(sls_context_class()->dtor(ctx));
  single_ctx = NULL;
}

void demo_exit() { demo_del_ctx(demo_shared_ctx()); }

slsContext *demo_shared_ctx()
{
  if (single_ctx) {
    return single_ctx;
  } else {
    slsContext *self = sls_context_new("demo", 640, 480);

    self->data = &data;

    self->dtor = NULL;

    self->setup = demo_ctx_setup;
    self->teardown = demo_ctx_teardown;
    self->update = demo_ctx_update;
    self->display = demo_ctx_display;

    self->handle_event = demo_handle_event;

    self->resize = demo_ctx_resize;
    single_ctx = self;

    atexit(demo_exit);
    return self;
  }
}

int demo_main(int *argcr, char **argv)
{
  slsContext *ctx = demo_shared_ctx();

  if (ctx) {
    sls_msg(ctx, run);
  }
  return 0;
}

//---------------------------------context
//callbacks---------------------------------------

void demo_ctx_setup(slsContext *self)
{
  sls_checkmem(self && self->state);
  sls_context_setup(self);

  demo_mv_setup(self);
  demo_scene_setup(self);

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  // glDisable(GL_DEPTH_TEST);

  glClearColor(0.1, 0.2, 0.4, 1.0);

  //glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND_COLOR);
  glCullFace(GL_BACK);

  return;
  error:
  SLS_DEBUGGER_BREAKPT();
  return;
}

static void demo_scene_setup(slsContext *self)
{

  slsShader *res = sls_shader_init(
      &data.phong_shader,
      sls_create_program("resources/shaders/demo.vert",
                         "resources/shaders/demo.frag",
                         "resources/shaders/uniforms.glsl"));
  res->owns_program = true;
  data.cube_mesh = sls_cube_mesh(false);

  slsVertex verts[] = {
      {.position = {-1.f, -1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = {-1.f, 1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = {1.f, 1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = {1.f, -1.f, 0.f}, .normal = {0.f, 0.f, 1.f}}};
  uint32_t idxs[] = {2, 1, 0, 3, 2, 0};
  data.rect_mesh =
      sls_mesh_new(verts, SLS_ARRAY_COUNT(verts), idxs, SLS_ARRAY_COUNT(idxs));

  assert(data.rect_mesh);
  sls_mesh_bind(data.rect_mesh, &data.phong_shader);
  sls_mesh_bind(data.cube_mesh, &data.phong_shader);

}

static void demo_mv_setup(slsContext *self)
{
  kmMat4Identity(&data.projection);
  kmMat4Identity(&data.camera_view);
}

void demo_ctx_teardown(slsContext *self)
{
  sls_shader_dtor(&data.phong_shader);
  free(sls_msg(data.cube_mesh, dtor));

  sls_lightbatch_dtor(&data.lights);

  free(sls_mesh_dtor(data.rect_mesh));

  sls_context_class()->teardown(self);
}

void demo_ctx_update(slsContext *self, double dt) {}

void demo_ctx_display(slsContext *self, double dt)
{

  kmMat4 mv, normal, inv_mv;
  // use matrices to perform modelview transformation
  {
    kmMat4Translation(&inv_mv, 0.0f, -0.5f, -1.f);
    kmVec3 v = {1.f, 0.5, 0.5};
    kmMat4RotationAxisAngle(&normal, &v, M_PI_2);
    kmMat4Multiply(&mv, &inv_mv, &normal);
  }

  kmMat4Inverse(&inv_mv, &mv);
  kmMat4Transpose(&normal, &inv_mv);

  sls_shader_bind_mat4(
      &data.phong_shader,
      (uint) glGetUniformLocation(data.phong_shader.program, "model_view"), &mv,
      false);

  sls_shader_bind_mat4(
      &data.phong_shader,
      (uint) glGetUniformLocation(data.phong_shader.program, "inv_model_view"),
      &inv_mv,
      false);

  sls_shader_bind_mat4(
      &data.phong_shader,
      (uint) glGetUniformLocation(data.phong_shader.program, "normal_mat"),
      &normal,
      false);

  if (data.rect_mesh) {
    //_sls_mesh_roughdraw(data.rect_mesh, data.phong_shader.program, dt);
  }
  if (data.cube_mesh) {
    _sls_mesh_roughdraw(data.cube_mesh, data.phong_shader.program, dt);
  }
}


void demo_ctx_resize(slsContext *self, int x, int y)
{
  glUseProgram(data.phong_shader.program);

  sls_context_class()->resize(self, x, y);

  const float fov = 70.f;
  float aspect = x / (float) y;
  // kmMat4PerspectiveProjection(&data.projection, fov, aspect, -1.f, 1.f);

  kmMat4OrthographicProjection(&data.projection, -5.f, 5.f, -5.f / aspect,
                               5.f / aspect, -100.f, 100.f);

  glUniformMatrix4fv(
      glGetUniformLocation(data.phong_shader.program, "projection"), 1,
      GL_FALSE, data.projection.mat);
}

void demo_handle_event(slsContext *self, SDL_Event const *e)
{
  sls_context_handle_event(self, e);

  switch (e->type) {
    case SDL_KEYDOWN: {
      SDL_KeyboardEvent const *ke = &e->key;
      switch (ke->keysym.scancode) {
        case SDL_SCANCODE_ESCAPE: {
          self->is_running = false;
        }
          break;

        default:
          break;
      }

    }
      break;

    default:
      break;
  }
}
