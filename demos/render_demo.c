//
// Created by Steven on 7/2/15.
//

#include "render_demo.h"
#include <assert.h>
#include <renderer/slsshader.h>


void demo_update_modelview(slsContext *self,
                           kmMat4 const *model) SLS_NONNULL(1, 2);


void demo_handle_event(slsContext *self,
                       const SDL_Event *e) SLS_NONNULL(1);


void demo_handle_event(slsContext *self,
                       const SDL_Event *e)
{

  sls_context_class()->handle_event(self, e);
  demoData *data = self->data;

  data->mouse_motion = (slsIPoint) {0, 0};
  data->camera_move_input = (slsIPoint) {0, 0};

  switch (e->type) {
    case (SDL_MOUSEMOTION): {
      SDL_MouseMotionEvent me = e->motion;
      if (me.state & SDL_MOUSEBUTTONDOWN) { // mouse button is down
        data->mouse_motion.x += me.xrel;
        data->mouse_motion.y -= me.yrel;
      }

      break;
    }

    default:
      break;
  }
}


void demo_context_setup(slsContext *self)
{
  if (!self) { exit(EXIT_FAILURE); }

  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);

  sls_checkmem(self->data);

  demoData *data = self->data;
  {
    const size_t initial_size = 16;
    data->model_view = *sls_matrix_stack_init(&data->model_view,
                                              initial_size);
  }

  sls_checkmem(data->model_view.matrices);


  demo_setup_shaders(self);

  demo_setup_textures(self);

  demo_setup_scene(self);


  int x, y;
  SDL_GetWindowSize(self->window, &x, &y);
  sls_msg(self, resize, x, y);

  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);
  return;
  error:
  exit(EXIT_FAILURE);

}

void demo_setup_shaders(slsContext *self)
{
  if (!self || !self->data) { exit(EXIT_FAILURE); }
  char const *fs_path = "resources/shaders/default.frag";
  char const *vs_path = "resources/shaders/default.vert";

  demoData *data = self->data;


  data->program = sls_create_program(vs_path, fs_path);
  data->shader =
      sls_shader_init(apr_pcalloc(self->state->pool,
                                  sizeof(slsShader)),
                      self->state->pool,
                      data->program);

}


void demo_setup_textures(slsContext *self)
{

  demoData *data = self->data;

  char const *img_path = "resources/free-textures/176.jpg";
  char const *norm_path = "resources/free-textures/176_norm.jpg";


  data->tex_obj = sls_texture_new(img_path, NULL, norm_path);

  data->tank_tex = sls_texture_new("resources/art/tankBeige_outline.png",
                                   "resources/art/tankBeige_specular.png",
                                   "resources/art/tankBeige_normal.png");

  data->barrel_tex = sls_texture_new("resources/art/barrelBeige_outline.png",
                                   "resources/art/barrelBeige_specular.png",
                                   "resources/art/barrelBeige_normal.png");

  data->tex = sls_gltex_from_file(img_path, -1, -1);
}

void demo_setup_scene(slsContext *self)
{
  demoData *data = self->data;


  glUseProgram(data->program);


  data->camera = (slsTransform2D) {.pos={0.0, 0.0}, .scale={1.0, 1.0}, .rot=0.0};

  sls_msg(data->tex_obj, set_program, data->program);


  self->state->root =
      sls_entity_new(self->pool, SLS_COMPONENT_NONE, "root");
  slsEntity *root = self->state->root;
  sls_checkmem(root);

  slsSprite *sprite = malloc(sizeof(slsSprite));
  sprite = sls_init_sprite(sprite,
                           self->state,
                           self->pool, "sprite_a",
                           data->tank_tex,
                           data->shader);


  sls_entity_addchild(root, sprite);


  return;

  error:
  assert(0);
  return;

}


void demo_camera_drag(slsContext *self, double dt)
{
  demoData *data = self->data;

  slsIPoint mm = data->mouse_motion;
  slsIPoint zero = {0, 0};
  kmVec2 translate = {0.0, 0.0};
  kmVec2 mouse_v = {0.0, 0.0};
  double move_speed = 50.0;
  bool moving = false;

  demo_update_uniforms(self, dt);


  if (!sls_ipoint_eq(&mm, &zero)) {

    double angle = atan2(mm.y, mm.x);
    translate = (kmVec2) {mm.x, mm.y};

    kmVec2Normalize(&translate, &translate);


    double full_speed = dt * move_speed;
    translate.x *= full_speed;
    translate.y *= full_speed;


    moving = true;
  }

  if (moving) {

    data->camera.pos.x += translate.x;
    data->camera.pos.y += translate.y;

  }

  data->mouse_motion = (slsIPoint) {0, 0};
}


void demo_context_update(slsContext *self, double dt)
{

  demo_camera_drag(self, dt);

  sls_entity_update(self->state->root, self->state, dt);

#if 0
  slsPlayerInput const *inp = &self->state->input;

  if (inp->key_up || inp->key_down || inp->key_left || inp->key_right) {
    sls_log_info(
        "input:\n"
            "directions: %s %s %s %s\n"
            "mouse_pos: %d %d\nvelocity %d %d\n",
        inp->key_up ? "up" : "",
        inp->key_down ? "down" : "",
        inp->key_left ? "left" : "",
        inp->key_right ? "right" : "",
        inp->mouse_pos.x, inp->mouse_pos.y,
        inp->mouse_vel.x, inp->mouse_vel.y);
  }
#endif // 0

}


void demo_update_uniforms(slsContext *self, double dt)
{
  demoData *data = self->data;
  if (data->uniforms.time_ >= 0) {
    glUniform1f(data->uniforms.time_, clock() / (float) CLOCKS_PER_SEC);
  }
}


void demo_context_display(slsContext *self, double dt)
{
  sls_context_class()->display(self, dt);

  demoData *data = self->data;

  GLint time = glGetUniformLocation(data->program, "time");

  float t = clock() / (float) CLOCKS_PER_SEC;
  glUniform1f(time, t);

  sls_entity_display(self->state->root, self->state, dt);

}

void demo_context_teardown(slsContext *self)
{
  demoData *data = self->data;

  if (data) {

    glDeleteProgram(data->program);
    glDeleteTextures(1, &data->tex);

    sls_msg(data->tex_obj, dtor);
    sls_msg(data->tank_tex, dtor);
    sls_msg(data->barrel_tex, dtor);

    sls_shader_dtor(data->shader);
    free(data);
  }

}

void demo_context_resize(slsContext *self, int x, int y)
{
  sls_context_class()->resize(self, x, y);
  demoData *data = self->data;
  glViewport(0, 0, x * 2, y * 2);

  if (x != 0 && y != 0) {
    kmMat4 projection;

    if (x > y) {
      float aspect = x / (float) y;

      kmMat4OrthographicProjection(&projection, -aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
    } else {
      float aspect = y / (float) x;
      kmMat4OrthographicProjection(&projection, -1, 1, -aspect, aspect, -10.0f, 10.0f);

    }

    glUniformMatrix4fv(data->shader->uniforms.projection, 1, GL_FALSE, projection.mat);
  }
}


int render_demo_main(int *argc, char **argv)
{
  size_t w = 640;
  size_t h = 480;
  slsContext *c = sls_context_new("window", w, h);

  c->data = NULL;

  c->setup = demo_context_setup;
  c->update = demo_context_update;
  c->display = demo_context_display;
  c->teardown = demo_context_teardown;
  c->resize = demo_context_resize;
  c->handle_event = demo_handle_event;


  assert(c);


  sls_msg(c, run);
  sls_msg(c, dtor);


  return 0;
}

void demo_update_modelview(slsContext *self,
                           kmMat4 const *model)
{

  demoData *data = self->data;
  assert(data);
  kmMat4 view = *data->model_view.matrices;
  kmMat4 model_view;


  kmMat4Multiply(&model_view, model, &view);


  glUniformMatrix4fv(data->uniforms.model_view, 1, GL_FALSE, model_view.mat);
}
