//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"
#include <kazmath/kazmath.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif //__EMSCRIPTEN__

#include "renderer/slsmodel.h"
#include "time.h"

static inline void setup()
{
#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__

};

typedef struct demoData
{
  GLuint program;
  slsMesh* mesh;
  slsModel* model;

  struct
  {
    GLint time_;
    GLint projection;
    GLint model_view;
    GLint normal_mat;
  } uniforms;

  kmMat4 model_view;
} demoData;

void demo_context_setup(slsContext* self)
{
  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);

  demoData* data = self->data;
  char const* fs_path;

  char const* vs_path;


#ifndef SLS_GLES

  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  if (major < 3 || (major == 3 && minor < 2)) {
    vs_path = "resources/shaders/default-legacy.vert";
    fs_path = "resources/shaders/default-legacy.frag";
  } else {
    vs_path = "resources/shaders/default.vert";
    fs_path = "resources/shaders/default.frag";
  }

#else
  vs_path = "resources/shaders/default-legacy.vert";
  fs_path = "resources/shaders/default-legacy.frag";
#endif


  data->program = sls_create_program(vs_path, fs_path);
  data->mesh = sls_mesh_create_shape("square");
  sls_checkmem(data->mesh);

  kmMat4 transform;
  data->model = sls_model_new(data->mesh, data->program, kmMat4Scaling(&transform, 0.5, 0.5, 1.0));

  sls_msg(data->mesh, bind, data->program);

  glUseProgram(data->program);

  data->uniforms.time_ = glGetUniformLocation(data->program, "time");
  data->uniforms.model_view = glGetUniformLocation(data->program, "model_view");
  data->uniforms.projection = glGetUniformLocation(data->program, "projection");
  data->uniforms.normal_mat = glGetUniformLocation(data->program, "normal_mat");


  int x, y;
  glfwGetWindowSize(self->window, &x, &y);
  sls_msg(self, resize, x, y);

  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);
  return;
error:
  getchar();
  exit(EXIT_FAILURE);
}

void demo_context_update(slsContext* self, double dt)
{
  float t = clock() / (float)CLOCKS_PER_SEC;

  kmMat4 rot, scale;
  *kmMat4Multiply(&((demoData*)self->data)->model->transform,
                  kmMat4RotationZ(&rot, t),
    kmMat4Scaling(&scale, 0.25, 0.25, 1.0));

  demoData* data = self->data;
}

void demo_context_display(slsContext* self, double dt)
{
  demoData* data = self->data;

  GLint time = glGetUniformLocation(data->program, "time");

  float t = clock() / (float)CLOCKS_PER_SEC;
  glUniform1f(time, t);

  kmMat4Identity(&data->model_view);
  sls_msg(data->model, push_transform, &data->model_view, data->uniforms.model_view);
  sls_msg(data->model, draw, GL_TRIANGLES, dt);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _sls_mesh_roughdraw(data->mesh, data->program, dt);

  glfwSwapBuffers(self->window);
}

void demo_context_teardown(slsContext* self)
{
  demoData* data = self->data;

  if (data) {
    if (data->mesh) {
      sls_msg(data->mesh, dtor);
    }
    if (data->model) {
      sls_msg(data->model, dtor);
    }
    glDeleteProgram(data->program);

    free(data);
  }
}

void demo_context_resize(slsContext* self, int x, int y)
{
  sls_context_class()->resize(self, x, y);
  demoData* data = self->data;

  float aspect = x / (float)y;

  kmMat4 projection;
  kmMat4OrthographicProjection(&projection, -aspect, aspect, -1, 1, -10.0, 10.0);
  glUniformMatrix4fv(data->uniforms.projection, 1, GL_FALSE, projection.mat);
}

int sls_main()
{
  slsContext* c = sls_context_new("window", 640, 480);

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


int main(int argc, char* argv[])
{
  return sls_main();
}

