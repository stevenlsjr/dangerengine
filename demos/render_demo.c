//
// Created by Steven on 7/2/15.
//

#include "render_demo.h"
#include "../src/dangerengine.h"

void demo_setup_textures(slsContext *pContext);

void demo_setup_shaders(slsContext *self);

void demo_setup_scene(slsContext *self);

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
  glfwGetWindowSize(self->window, &x, &y);
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


}


void demo_context_display(slsContext *self, double dt)
{


  demoData *data = self->data;

  GLint time = glGetUniformLocation(data->program, "time");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, data->tex);

  float t = clock() / (float) CLOCKS_PER_SEC;
  glUniform1f(time, t);

  kmMat4 view;
  kmMat4Identity(&view);

  sls_msg(data->mesh, pre_draw, data->program, dt);

  for (int i = 0; i < data->n_models; ++i) {
    slsModel *model = data->models[i];
    if (!model) { continue; }

    kmMat4 model_view;
    kmMat4Multiply(&model_view, &view, &model->transform);

    glUniformMatrix4fv(data->uniforms.model_view, 1, GL_FALSE, model_view.mat);

    sls_msg(model->mesh, draw, dt);
  }

  sls_msg(data->mesh, post_draw, data->program, dt);


  glfwSwapBuffers(self->window);
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

  float aspect = x / (float) y;

  kmMat4 projection;
  kmMat4OrthographicProjection(&projection, -aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
  glUniformMatrix4fv(data->uniforms.projection, 1, GL_FALSE, projection.mat);
}


int render_demo_main(int *argc, char **argv)
{
  slsContext *c = sls_context_new("window", 640, 480);

  c->data = NULL;

  c->setup = demo_context_setup;
  c->update = demo_context_update;
  c->display = demo_context_display;
  c->teardown = demo_context_teardown;
  c->resize = demo_context_resize;

  assert(c);
  sls_msg(c, run);
  sls_msg(c, dtor);


  return 0;
}
