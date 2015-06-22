//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"
#include <assert.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#endif //__EMSCRIPTEN__

void demo_display(slsContext *self, double dt);
void demo_update(slsContext *self, double dt);

static float square_verts[] = {
  -1.0f, -1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f, 0.0f,
   1.0f, -1.0f, 0.0f
};

static float square_normals[] = {
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f
};

static float square_uvs[] = {
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 0.0f
};

static int square_idx[] = {0, 1, 2, 3, 2};


static GLint program;
static GLuint vbo, ibo;

void demo_update(slsContext *self, double dt)
{
  static slsBool color_set = SLS_FALSE;
  if (!color_set) {
    color_set = SLS_TRUE;

    glClearColor(0.2, 0.3, 0.35, 1.0);
  }
}

void demo_display(slsContext *self, double dt)
{
  if (!self || !self->priv) { return; }
  glClear(GL_COLOR_BUFFER_BIT);


  glfwSwapBuffers(self->window);
}

void demo_setup(){}
void demo_teardown(){}


int sls_main()
{
  sls_sleep((size_t) (1.2 * CLOCKS_PER_SEC));
  slsContext *c = sls_context_new("window", 640, 640);

  assert(c);

  c->display = demo_display;
  c->update = demo_update;

  demo_setup();
  sls_msg(c, run);
  demo_teardown();
  sls_msg(c, dtor);


  return 0;
}


int main(int argc, char *argv[])
{

#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__

  return sls_main();
}

