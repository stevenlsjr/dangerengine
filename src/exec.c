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
  slsMesh *mesh;
} demoData;

void demo_context_setup(slsContext *self)
{
  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);

  demoData *data = self->data;
  char const *vs_path = "resources/shaders/default.vert";
  char const *fs_path = "resources/shaders/default.frag";

  data->program = sls_create_program(vs_path, fs_path);

  slsVertex verts[] = {
    (slsVertex){.position={-1.0, -1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={0.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}},
    (slsVertex){.position={ 0.0,  1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={0.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}},
    (slsVertex){.position={ 1.0, -1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={0.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}}
  };

  unsigned inds[] = {0, 1, 2};
  data->mesh = sls_mesh_new(verts, sizeof(verts)/sizeof(slsVertex), inds, sizeof(inds)/sizeof(unsigned));
  sls_msg(data->mesh, bind, data->program);


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

  glBindVertexArray(data->mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, data->mesh->vbo);



  glDrawArrays(GL_TRIANGLES, 0, (GLsizei)sls_msg(data->mesh->indices, length));


  glfwSwapBuffers(self->window);
  glUseProgram(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);



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

