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

typedef struct {
  GLuint program;
  GLuint vbo;
  GLuint ibo;
  GLuint vao;

  struct {
    GLuint position;
  } atribs;
} demoData;

void demo_context_setup(slsContext *self)
{
  sls_context_class()->setup(self);

  demoData *data = self->data;
  char const *vs_path = "resources/shaders/default.vert.glsl";
  char const *fs_path = "resources/shaders/default.frag.glsl";

  data->program = sls_create_program(vs_path, fs_path);
  data->atribs.position = 0;

  glUseProgram(data->program);

  glGenBuffers(1, &data->vbo);
  glGenBuffers(1, &data->ibo);

  glGenVertexArrays(1, &data->vao);



  // make a triangle
  float verts[]={
    -1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
  };
  const int vert_dim = 3;

  glBindVertexArray(data->vao);
  {
    glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(data->atribs.position);

    sls_log_info("%d", data->atribs.position);

    glVertexAttribPointer(
      data->atribs.position,
      vert_dim,
      GL_FLOAT,
      GL_FALSE,
      0,
      (GLvoid *)0
    );
    glBindVertexArray(0);
    glDisableVertexAttribArray(data->atribs.position);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glUseProgram(0);

  glClearColor(0.1, 0.24, 0.3, 1.0);
}

void demo_context_update(slsContext *self, double dt)
{
  demoData *data = self->data;
}

void demo_context_display(slsContext *self, double dt)
{
  demoData *data = self->data;


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glUseProgram(data->program);
  // setup vert position pointer

  glBindVertexArray(data->vao);

  glEnableVertexAttribArray(data->atribs.position);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwSwapBuffers(self->window);
  glUseProgram(0);
  glBindVertexArray(0);


}

void demo_context_teardown(slsContext *self) {
  demoData *data = self->data;

  GLuint buffers[] = {data->vbo, data->ibo};
  glDeleteBuffers(2, buffers);
  glDeleteVertexArrays(1, &data->vao);

  glDeleteProgram(data->program);
}

int sls_main()
{
  slsContext *c = sls_context_new("window", 640, 640);

  demoData *data = calloc(sizeof(demoData), 1);
  c->data = data;

  c->setup = demo_context_setup;
  c->update = demo_context_update;
  c->display = demo_context_display;
  c->teardown = demo_context_teardown;

  assert(c);
  sls_msg(c, run);
  sls_msg(c, dtor);

  free(data);

  return 0;
}


int main(int argc, char *argv[])
{


  return sls_main();
}

