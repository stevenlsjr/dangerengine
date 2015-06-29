//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"
#include <kazmath/kazmath.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif //__EMSCRIPTEN__


static inline void setup()
{
#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__
};

typedef struct demoData {
  GLuint program;
  slsMesh *mesh;

  struct {
    GLint time_;
    GLint projection;
    GLint model_view;
    GLint normal_mat;
  } uniforms;
} demoData;

void demo_context_setup(slsContext *self)
{
  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);

  demoData *data = self->data;
  char const *vs_path = "resources/shaders/default.vert";
  char const *fs_path = "resources/shaders/default.frag";

  data->program = sls_create_program(vs_path, fs_path);
  data->mesh = sls_mesh_create_shape("square");
  sls_checkmem(data->mesh);

  sls_msg(data->mesh, bind, data->program);

  glUseProgram(data->program);

  data->uniforms.time_ = glGetUniformLocation(data->program, "time");
  data->uniforms.model_view = glGetUniformLocation(data->program, "model_view");
  data->uniforms.projection = glGetUniformLocation(data->program, "projection");
  data->uniforms.normal_mat = glGetUniformLocation(data->program, "normal_mat");



  // setup projection matrix
  kmMat4 projection;
  kmMat4OrthographicProjection(&projection,
                               -2.0, 2.0,
                               -2.0, 2.0,
                               -10.0, 10.0);

  glUniformMatrix4fv(data->uniforms.projection,
                     16, GL_FALSE,
                     projection.mat);

  kmMat4OrthographicProjection(&projection, -20.0, 20.0, -20.0, 20.0, -10.0, 10.0);
  glUniformMatrix4fv(data->uniforms.projection, 1, GL_FALSE, projection.mat);

  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);
  return;
error:
  getchar();
  exit(EXIT_FAILURE);
}

void demo_context_update(slsContext *self, double dt)
{
  demoData *data = self->data;
}

void demo_context_display(slsContext *self, double dt)
{
  demoData *data = self->data;

  GLint time = glGetUniformLocation(data->program, "time");

  float t = clock()/(float)CLOCKS_PER_SEC;
  glUniform1f(time, t);


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _sls_mesh_roughdraw(data->mesh, data->program);

  glfwSwapBuffers(self->window);


}

void demo_context_teardown(slsContext *self) {
  demoData *data = self->data;

  if (data) {
    if (data->mesh) {
      sls_msg(data->mesh, dtor);
    }
    free(data);
  }

  glDeleteProgram(data->program);
}

int sls_main()
{
  slsContext *c = sls_context_new("window", 640, 640);

  c->data = NULL;

  c->setup = demo_context_setup;
  c->update = demo_context_update;
  c->display = demo_context_display;
  c->teardown = demo_context_teardown;

  assert(c);
  sls_msg(c, run);
  sls_msg(c, dtor);


  return 0;
}


int main(int argc, char *argv[])
{


  return sls_main();
}

