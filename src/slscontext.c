//
// Created by Steven on 4/25/15.
//

#include "slscontext.h"
#include "slsutils.h"
#include <GLFW/glfw3.h>



#include <stdlib.h>
#include <stdio.h>

#include "sls-handlers.h"

struct slsContext_p {
  clock_t last;
  clock_t dt_acc;
};

void sls_context_run(slsContext *self);

void sls_context_iter(slsContext *self);



void sls_context_resize(slsContext *self, size_t x, size_t y);

void sls_context_update(slsContext *self, double dt);

void sls_context_display(slsContext *self, double dt);

slsContext * sls_context_init(slsContext *self,
                              char const *caption,
                              size_t width,
                              size_t height);



void sls_context_dtor(slsContext *self);

static const slsContext sls_context_proto = {
  .init = sls_context_init,
  .dtor = sls_context_dtor,

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


slsContext const *sls_context_class() {

    return &sls_context_proto;
}

slsContext *sls_context_new(char const *caption, size_t width, size_t height)
{
  
  slsContext *self = sls_objalloc(sls_context_class(), sizeof(slsContext));


  return self->init(self, caption, width, height);
}


slsContext *sls_context_init(slsContext *self,
                             char const *caption,
                             size_t width,
                             size_t height)
{
  if (!self) { return NULL; }
  *self = sls_context_proto;

  if (!sls_is_active()) {
      bool res = sls_init();
  }

  char const *capA = caption ? caption : "";

  self->window = glfwCreateWindow((int)width, (int)height, capA, NULL, NULL);
  sls_check(self->window, "window creation failed");

  self->priv = calloc(1, sizeof(slsContext_p));
  sls_checkmem(self->priv);

  *(self->priv) = (slsContext_p) {.dt_acc=0, .last=0};

  return self;
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
  if (!self || !self->priv) {return;}
  self->priv->last = clock();
  self->priv->dt_acc = 0;

  sls_bind_context(self);
  self->is_running = SLS_TRUE;
  while(self->is_running) {
    sls_msg(self, iter);
  }

}

void sls_context_iter(slsContext *self)
{
  if (!self || !self->priv) {
    return;
  }
  clock_t now = clock();
  slsContext_p *priv = self->priv;
  priv->dt_acc += now - priv->last;
  priv->last = now;


  if (priv->dt_acc > self->interval) {
    double dt = priv->dt_acc / (double)CLOCKS_PER_SEC;
    priv->dt_acc = 0;
    sls_msg(self, update, dt);
    sls_msg(self, display, dt);
  }

  glfwPollEvents();
  if (glfwWindowShouldClose(self->window)) {
    self->is_running = SLS_FALSE;
  }
}



void sls_context_resize(slsContext *self, size_t x, size_t y)
{
  glViewport(0, 0, (int)x, (int)y);
}

void sls_context_update(slsContext *self, double dt){}

void sls_context_display(slsContext *self, double dt)
{
  if (!self || !self->priv) { return; }
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(self->window);
}