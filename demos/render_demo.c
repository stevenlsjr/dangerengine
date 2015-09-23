//
// Created by Steven on 7/2/15.
//

#include "render_demo.h"
#include "../src/dangerengine.h"

void demo_setup_textures(slsContext *pContext);

void demo_setup_shaders(slsContext *self);

void demo_setup_scene(slsContext *self);



void demo_update_modelview(slsContext *self,
                           kmMat4 const *model) SLS_NONNULL(1, 2);


void demo_handle_event(slsContext *self, const SDL_Event *e) SLS_NONNULL(1);



void demo_handle_event(slsContext *self, const SDL_Event *e)
{
  sls_context_class()->handle_event(self, e);
  demoData* data =self->data;
  switch (e->type) {
    case (SDL_MOUSEMOTION): {
      SDL_MouseMotionEvent me = e->motion;
      if (me.state & SDL_MOUSEBUTTONDOWN) { // mouse button is down
        data->mouse_motion = (slsIPoint){me.xrel, me.yrel};
      } else {
        data->mouse_motion = (slsIPoint){0, 0};
      }

      data->mouse_p = (slsIPoint) {me.x, me.y};

    }
      break;
    default:
      break;
  }
}

void demo_add_model(demoData *data, float x, float y)
{
  if (data->n_models >= DEMO_MAX_MODELS) { return; }
  kmMat4 mat, scale;
  kmMat4Translation(&mat, x, y, 0);

  kmMat4Scaling(&scale, 0.25, 0.25, 1.0);
  kmMat4Multiply(&mat, &mat, &scale);

  slsModel *model = sls_model_new(data->mesh, data->program, &mat);
  sls_checkmem(model);

  data->models[data->n_models++] = model;


  return;

  error:
  if (model && model->dtor) {
    sls_msg(model, dtor);
  }

}

void demo_uniform_locations(demoData *data)
{
  assert(data);
  data->uniforms.time_ = glGetUniformLocation(data->program, "time");
  data->uniforms.model_view = glGetUniformLocation(data->program, "model_view");
  data->uniforms.projection = glGetUniformLocation(data->program, "projection");
  data->uniforms.normal_mat = glGetUniformLocation(data->program, "normal_mat");
  data->uniforms.tex_sample = glGetUniformLocation(data->program, "diffuse_map");
}


void demo_setup_shaders(slsContext *self)
{
  if (!self || !self->data) { exit(EXIT_FAILURE); }
  char const *fs_path = "resources/shaders/default.frag";
  char const *vs_path = "resources/shaders/default.vert";

  demoData *data = self->data;


  data->program = sls_create_program(vs_path, fs_path);

}

void demo_context_setup(slsContext *self)
{
  if (!self) { exit(EXIT_FAILURE); }

  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);


  demo_setup_shaders(self);

  demo_setup_textures(self);

  demo_setup_scene(self);


  int x, y;
  SDL_GetWindowSize(self->window, &x, &y);
  sls_msg(self, resize, x, y);

  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);
  return;

}

void demo_setup_scene(slsContext *self)
{
  demoData *data = self->data;

  data->mesh = sls_mesh_create_shape("square");
  sls_checkmem(data->mesh);

  sls_msg(data->mesh, bind, data->program);

  glUseProgram(data->program);

  demo_uniform_locations(data);

  kmMat4Identity(&data->view_matrix);


  sls_msg(data->tex_obj, set_program, data->program);

  demo_add_model(data, 0.0f, 0.0f);
  demo_add_model(data, -1.0f, 0.30f);

  return;
error:
  getchar();
  exit(EXIT_FAILURE);
}

void demo_setup_textures(slsContext *self)
{

  demoData *data = self->data;

  char const *img_path = "resources/free-textures/151.jpg";
  char const *norm_path = "resources/free-textures/151.jpg";

  data->tex_obj = sls_texture_new(img_path, NULL, norm_path);

  data->tex = sls_gltex_from_file(img_path, -1, -1);
}

void demo_context_update(slsContext *self, double dt)
{
  demoData *data = self->data;

  slsIPoint mm = data->mouse_motion;
  if (mm.x != 0 && mm.y != 0) {

    double speed = 50;

    int window_y_direction = -1;

    kmVec2 mouse_v = {(float)mm.x, (float)-mm.y};
    kmVec2Scale(&mouse_v, &mouse_v, dt);
    sls_log_info("%f %f", mouse_v.x, mouse_v.y);
  }


}


void demo_context_display(slsContext *self, double dt)
{


  demoData *data = self->data;

  GLint time = glGetUniformLocation(data->program, "time");



  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, data->tex);

  float t = clock() / (float) CLOCKS_PER_SEC;
  glUniform1f(time, t);



  sls_msg(data->mesh, pre_draw, data->program, dt);

  for (int i = 0; i < data->n_models; ++i) {
    slsModel *model = data->models[i];
    if (!model) { continue; }

    demo_update_modelview(self, &model->transform);

    sls_msg(model->mesh, draw, dt);
  }

  sls_msg(data->mesh, post_draw, data->program, dt);


  glBindTexture(GL_TEXTURE_2D, 0);
}

void demo_context_teardown(slsContext *self)
{
  demoData *data = self->data;

  if (data) {
    if (data->mesh) {
      sls_msg(data->mesh, dtor);
    }

    glDeleteProgram(data->program);
    glDeleteTextures(1, &data->tex);

    for (int i = 0; i < data->n_models; ++i) {
      slsModel *model = data->models[i];
      if (model) { sls_msg(model, dtor); }

    }

    free(data);
  }
}

void demo_context_resize(slsContext *self, int x, int y)
{
  sls_context_class()->resize(self, x, y);
  demoData *data = self->data;


  if (x != 0 && y!= 0) {
    kmMat4 projection;

    if (x > y) {
      float aspect = x / (float) y;

      kmMat4OrthographicProjection(&projection, -aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
    } else {
      float aspect = y / (float) x;
      kmMat4OrthographicProjection(&projection, -1, 1, -aspect, aspect, -10.0f, 10.0f);

    }

    glUniformMatrix4fv(data->uniforms.projection, 1, GL_FALSE, projection.mat);
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
  kmMat4 model_view;
  kmMat4Multiply(&model_view, model, &data->view_matrix);


  glUniformMatrix4fv(data->uniforms.model_view, 1, GL_FALSE, model_view.mat);
}