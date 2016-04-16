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

  slsMesh *mesh;


  slsTrackball trackball;

  kmMat4 camera_view;
  kmMat4 projection;




} DemoData;


struct {
  GLuint fbo;
  GLuint texture;
} frame_buffer;

static
void demo_move_trackball(slsContext *self, slsIPoint const *start_point, slsIPoint const *second_point,
                         slsIPoint const *window_size);

static
void demo_mv_setup(slsContext *self);

static
void demo_scene_setup(slsContext *pContext);

static DemoData data = {

};

static slsContext *single_ctx = NULL;

void demo_del_ctx(slsContext *ctx)
{
  assert(ctx == single_ctx);


  free(sls_context_class()->dtor(ctx));
  single_ctx = NULL;
}

void demo_exit()
{
  demo_del_ctx(demo_shared_ctx());
}


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


//---------------------------------context callbacks---------------------------------------

void demo_ctx_setup(slsContext *self)
{
  sls_checkmem(self && self->state);
  self->state->active_shader = NULL;
  sls_context_setup(self);

  demo_mv_setup(self);
  demo_scene_setup(self);


  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  //glDisable(GL_DEPTH_TEST);

  glClearColor(0.1, 0.2, 0.4, 1.0);

  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND_COLOR);
  glCullFace(GL_BACK);


  return;
  error:
  SLS_DEBUGGER_BREAKPT();
  return;
}

static
void demo_scene_setup(slsContext *self)
{

  slsShader *res = sls_shader_init(&data.phong_shader,
                                   sls_create_program("resources/shaders/demo.vert",
                                                      "resources/shaders/demo.frag",
                                                      "resources/shaders/demo_uniforms.glsl"));
  res->owns_program = true;
  data.mesh = sls_parametric_sphere_mesh(10);


  assert (res);

}

static
void demo_mv_setup(slsContext *self)
{
  kmMat4Identity(&data.projection);
  kmMat4Identity(&data.camera_view);

  sls_lightbatch_init(&data.lights, 8, (slsLight[]) {}, 1);


  sls_trackball_init(&data.trackball, 4.f, 2.f);
}

void demo_setup_framebuffer(slsContext *self)
{

#if 0 // TODO setup quad mesh to render framebuffer texture
  slsVertex verts[] = {
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}}
  };
  uint32_t indices[] = {0, 1, 2, 3, 2, 0};

  size_t n_verts = sizeof(verts) / sizeof(*verts);
  size_t n_indices = sizeof(indices) / sizeof(*indices);
  data.fb_target = sls_mesh_new(verts, n_verts, indices, n_indices);

  slsShader *res = sls_shader_init(&data.fb_shader, sls_create_program("resources/shaders/postprocess.vert",
                                                                       "resources/shaders/postprocess.frag",
                                                                       "resources/shaders/demo_uniforms.glsl"));
  sls_check(res, "posprocess shader didn't initialzie");

  sls_mesh_bind(data.fb_target, &data.fb_shader);

  return;
  error:
  exit(EXIT_FAILURE);

#endif
}


void demo_ctx_teardown(slsContext *self)
{
  sls_shader_dtor(&data.phong_shader);
  free(sls_msg(data.mesh, dtor));

  sls_lightbatch_dtor(&data.lights);


  sls_context_class()->teardown(self);
}


void demo_ctx_update(slsContext *self, double dt)
{

  data.lights.light_positions[0] = (kmVec4) {0.0, 0.1, 1.0, 0.0};

  GLsizei n_lights = (GLsizei) data.lights.n_lights;

  slsShader *s = &data.phong_shader;
  GLuint program = s->program;

  glUniform1i(glGetUniformLocation(program, "lights.n_lights"), (GLint) data.lights.n_lights);

  glUniform3fv(glGetUniformLocation(program, "lights.ambient_products"),
               n_lights,
               (GLfloat *) data.lights.ambient_products);
  glUniform3fv(glGetUniformLocation(program, "lights.specular_products"),
               n_lights,
               (GLfloat *) data.lights.specular_products);

  glUniform3fv(glGetUniformLocation(program, "lights.diffuse_products"),
               n_lights,
               (GLfloat *) data.lights.diffuse_products);
  glUniform4fv(glGetUniformLocation(program, "lights.light_positions"),
               n_lights,
               (GLfloat *) data.lights.light_positions);

  glUniformMatrix4fv(glGetUniformLocation(program, "lights.light_modelview"),
                     n_lights,
                     GL_FALSE,
                     (GLfloat *) data.lights.light_modelviews[0].mat);


}


void demo_ctx_display(slsContext *self, double dt)
{
  slsVertex verts[] = {
      {.position = {-1.f, -1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = {-1.f,  1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = { 1.f,  1.f, 0.f}, .normal = {0.f, 0.f, 1.f}},
      {.position = { 1.f, -1.f, 0.f}, .normal = {0.f, 0.f, 1.f}}
  };
  uint32_t idxs[] = {2, 1, 0, 3, 2, 0};

  slsMesh m;
  sls_mesh_init(&m, verts, sizeof(verts)/sizeof(*verts), idxs, sizeof(idxs)/sizeof(*idxs));

  sls_mesh_bind(&m, &data.phong_shader);

  kmMat4 id;
  kmMat4Identity(&id);
  sls_shader_bind_mat4(&data.phong_shader,
                       glGetUniformLocation(data.phong_shader.program, "model_view"), &id, false);

  _sls_mesh_roughdraw(&m, data.phong_shader.program, dt );



  sls_mesh_dtor(&m);

}


void demo_ctx_resize(slsContext *self, int x, int y)
{
  glUseProgram(data.phong_shader.program);

  sls_context_class()->resize(self, x, y);

  const float fov = 70.f;
  float aspect = x / (float) y;
  kmMat4PerspectiveProjection(&data.projection, fov, aspect, -1.f, 1.f);

  //kmMat4OrthographicProjection(&data.projection, -5.f, 5.f, -5.f/aspect, 5.f/aspect, -100.f, 100.f);

  glUniformMatrix4fv(glGetUniformLocation(data.phong_shader.program, "projection"),
                     1, GL_FALSE, data.projection.mat);

}

void demo_handle_event(slsContext *self, SDL_Event const *e)
{

  static slsIPoint last_mouse = {-1, -1};


  sls_context_handle_event(self, e);

  switch (e->type) {
    case SDL_MOUSEMOTION: {

      SDL_MouseMotionEvent const *me = &e->motion;
      slsIPoint mouse = {me->x, me->y};

      if (me->state & SDL_BUTTON(SDL_BUTTON_LEFT)) {

        if (last_mouse.x == -1 && last_mouse.y == -1) {
          last_mouse = mouse;
        }
        bool no_motion = sls_ipoint_eq(&mouse, &last_mouse);

        if (!no_motion) {
          slsIPoint win_size = {};
          SDL_GetWindowSize(self->window, &win_size.x, &win_size.y);
          demo_move_trackball(self, &mouse, &last_mouse, &win_size);

        }
      }

      last_mouse = mouse;
    }
      break;

    case SDL_KEYDOWN: {
      SDL_KeyboardEvent const *ke = &e->key;
      switch (ke->keysym.scancode) {
        case SDL_SCANCODE_R: {
          kmMat4Identity(&data.trackball.rotation_mat);
          kmQuaternionIdentity(&data.trackball.rotation);
        }
          break;

        case SDL_SCANCODE_COMMA: {

        }
          break;
        case SDL_SCANCODE_PERIOD: {
        }
          break;

        case SDL_SCANCODE_SPACE: {
        }
          break;

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

static
void demo_move_trackball(slsContext *self,
                         slsIPoint const *start_point,
                         slsIPoint const *second_point,
                         slsIPoint const *window_size)
{

  float
      w = window_size->x,
      h = window_size->y;


  kmVec2 delta = {
      .x = (start_point->x - second_point->x) / w,
      .y = (second_point->y - start_point->y) / h
  };


  slsTrackball *ball = &data.trackball;

  kmVec2 param_0 = {(2.f * start_point->x - w) / w, (h - 2.f * start_point->y) / h};
  kmVec2 param_1 = {(2.f * second_point->x - w) / w, (h - 2.f * second_point->y) / h};


  sls_trackball_set(ball, param_0, param_1);
}
