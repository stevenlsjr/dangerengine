/**
 * @file slscontext.c
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea.
**/
#include "sls-gl.h"

#include "slscontext.h"
#include "slsutils.h"


#include <stdlib.h>
#include <stdio.h>

#include "contexthandlers.h"


#ifdef EMSCRIPTEN
#   include <emscripten.h>
#endif //EMSCRIPTEN

struct slsContext_p {
  clock_t last;
  clock_t dt_acc;
};


/*----------------------------------------*
 * slsContext default method prototypes
 *----------------------------------------*/
void sls_context_run(slsContext *self);
void sls_context_iter(slsContext *self);
void sls_context_resize(slsContext *self, int x, int y);
void sls_context_update(slsContext *self, double dt);
void sls_context_display(slsContext *self, double dt);
slsContext *sls_context_init(slsContext *self,
                             char const *caption,
                             size_t width,
                             size_t height);

void sls_context_setup(slsContext *self);
void sls_context_teardown(slsContext *self);
void sls_context_dtor(slsContext *self);

/*----------------------------------------*
 * slsContext static prototype
 *----------------------------------------*/
static const slsContext sls_context_proto = {
  .init = sls_context_init,
  .dtor = sls_context_dtor,

  .setup = sls_context_setup,
  .teardown = sls_context_teardown,

  .run = sls_context_run,
  .iter = sls_context_iter,
  .resize = sls_context_resize,

  .update = sls_context_update,
  .display = sls_context_display,

  .is_running = SLS_FALSE,
  .interval = 50,
  .priv = NULL,
  .window = NULL
};



/*----------------------------------------*
 * slsContext class functions
 *----------------------------------------*/

slsContext const *sls_context_class()
{
  return &sls_context_proto;
}

slsContext *sls_context_new(char const *caption, size_t width, size_t height)
{

  slsContext *self = sls_objalloc(sls_context_class(), sizeof(slsContext));

  

  return self->init(self, caption, width, height);
}


/*----------------------------------------*
 * slsContext instance methods
 *----------------------------------------*/

slsContext *sls_context_init(slsContext *self,
                             char const *caption,
                             size_t width,
                             size_t height)
{

  // nullptr check and initialize class values
 if (!self) { return NULL; }
  *self = *sls_context_class();

  // initialize libraries if not active
  if (!sls_is_active()) {
    bool res = sls_init();
    sls_check(res, "initialization failed!");
  }

  // GLFW hints
  int hints [] = {
    GLFW_DOUBLEBUFFER, GL_TRUE,
    GLFW_CONTEXT_VERSION_MAJOR, 4,
    GLFW_CONTEXT_VERSION_MINOR, 1,
    GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE,
    GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE
  };

  size_t n_hints = sizeof(hints)/sizeof(int);
  sls_check(n_hints %2 == 0, "you must include an even number of glfw hint values");
  for (int i=0; i<n_hints/2; ++i){
    const int ii = i * 2;
    glfwWindowHint(hints[ii], hints[ii+1]);
  }


  // create glfw window
  self->window = glfwCreateWindow((int) width, (int) height, caption, NULL, NULL);
  
  sls_check(self->window, "window creation failed");

  // allocate and initialize private members
  self->priv = calloc(1, sizeof(slsContext_p));
  sls_checkmem(self->priv);
  *(self->priv) = (slsContext_p) {.dt_acc=0, .last=0};

  return self;

  /// exception
  error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  else if (self) {
    free(self);
  }
  return NULL;
}

void sls_context_dtor(slsContext *self)
{
  if (self) {
    if (self->priv) {
      free(self->priv);
    }
    free(self);
  }
}


void sls_context_run(slsContext *self)
{
  if (!self || !self->priv) { return; }
  self->priv->last = clock();
  self->priv->dt_acc = 0;

  self->is_running = SLS_TRUE;

  sls_msg(self, setup);

# ifndef EMSCRIPTEN
  // use a simple run loop (loop execution encapsulated in iter method)
  while (self->is_running) {
    sls_msg(self, iter);
  }
# else
  emscripten_set_main_loop_arg(sls_emscripten_loop, self, -1, SLS_FALSE);
# endif //!EMSCRIPTEN

  sls_msg(self, teardown);
}

void sls_emscripten_loop(void *vctx)
{
  slsContext *ctx = vctx;
  sls_msg(ctx, iter);
}

void sls_context_iter(slsContext *self)
{
  if (!self || !self->priv) {
    return;
  }

  // ensure that windows sets correct size at start of program
  static slsBool first_resize = SLS_FALSE;

  if (!first_resize) {
    int width, height;
    glfwGetFramebufferSize(self->window, &width, &height);
    sls_msg(self, resize, width, height);
  }

  clock_t now = clock();
  slsContext_p *priv = self->priv;
  priv->dt_acc += now - priv->last;
  priv->last = now;


  if (priv->dt_acc > self->interval) {

    double dt = priv->dt_acc / (double) CLOCKS_PER_SEC;
    priv->dt_acc = 0;
    sls_msg(self, update, dt);
    sls_msg(self, display, dt);
  }

  glfwPollEvents();
  if (glfwWindowShouldClose(self->window)) {
    self->is_running = SLS_FALSE;
  }
}


void sls_context_resize(slsContext *self, int x, int y)
{
  glViewport(0, 0, (int) x, (int) y);
}

void sls_context_update(slsContext *self, double dt) {
  sls_log_info("update dt: %f", dt);
}

void sls_context_display(slsContext *self, double dt)
{



  if (!self || !self->priv) { return; }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glfwSwapBuffers(self->window);
}

void sls_context_setup(slsContext *self)
{
  sls_bind_context(self);

# ifndef SLS_NOGLEW
  {
    glewExperimental = GL_TRUE;
    GLenum glew = glewInit();
    if (glew != GLEW_OK) {
      sls_log_err("glew error: %s", glewGetErrorString(glew));
      self->is_running = SLS_FALSE;
    }
    sls_log_info("glew version %s", glewGetString(GLEW_VERSION));
  }
# endif// !SLS_NOGLEW

  sls_log_info("openGL version %s", glGetString(GL_VERSION));

  // setup opengl pipeline

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);


}

void sls_context_teardown(slsContext *self)
{
  sls_unbind_context();
}

