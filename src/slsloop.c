//
// Created by Steven on 4/27/15.
//

#include "slsloop.h"
#include "slsutils.h"
#include "sls-handlers.h"
#include <stdio.h>
#include <stdlib.h>

slsMainLoop *sls_mainloop_init(slsMainLoop *self, slsContext *ctx);

void sls_mainloop_dtor(slsMainLoop *self);

void sls_mainloop_run(slsMainLoop *self);

void sls_mainloop_handle_events(slsMainLoop *self, double dt);

void sls_mainloop_update(slsMainLoop *self, double dt);

void sls_mainloop_display(slsMainLoop *self, double dt);

struct slsMainLoop_p {
  slsContext *ctx;
};

static const slsMainLoop sls_mainloop_proto = {
  .init = sls_mainloop_init,
  .dtor = sls_mainloop_dtor,
  .run = sls_mainloop_run,
  .update = sls_mainloop_update,
  .display = sls_mainloop_display,
  .is_running = false,
  .interval = 20,
  .priv = NULL
};

static slsMainLoop *sls_active_loop = NULL;

const slsMainLoop *sls_mainloop_class()
{
  return &sls_mainloop_proto;
}

slsMainLoop *sls_mainloop_new(slsContext *ctx)
{
  slsMainLoop *self = sls_objalloc(sls_mainloop_class(), sizeof(slsMainLoop));
  return sls_msg(self, init, ctx);
}

void sls_mainloop_dtor(slsMainLoop *self)
{
  if (!self) { return; }
  if (self->priv && self->priv->ctx) {
    sls_msg(self->priv->ctx, dtor);
    free(self->priv);
  }
  free(self);
}

slsMainLoop *sls_mainloop_init(slsMainLoop *self, slsContext *ctx)
{
  sls_checkmem(self);
  sls_checkmem(ctx);

  self->priv = calloc(1, sizeof(slsMainLoop_p));
  sls_checkmem(self->priv);

  self->priv->ctx = ctx;

  return self;

  error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  return NULL;
}


void sls_mainloop_run(slsMainLoop *self)
{
  sls_checkmem(self);
  sls_checkmem(self->priv);
  sls_checkmem(self->priv->ctx);

  clock_t last = clock();
  clock_t now = last;
  clock_t dt = 0;
  self->is_running = true;

  slsContext *ctx = self->priv->ctx;

  sls_bind_context(ctx);


  while (self->is_running) {
    now = clock();
    dt += now - last;
    last = now;
    
    if (dt >= self->interval) {
      double ddt = dt / (double) CLOCKS_PER_SEC;
      dt = 0;
      sls_msg(self, update, ddt);
      sls_msg(self, display, ddt);
    }

    glfwPollEvents();

    if (glfwWindowShouldClose(ctx->window)) {
      self->is_running = false;
      
    }
  }

  sls_active_loop = NULL;
  return;
error:
  return;
  sls_log_err("error label reached %s", __func__);
}

void sls_mainloop_update(slsMainLoop *self, double dt)
{

}

void sls_mainloop_display(slsMainLoop *self, double dt)
{
  if (!self->priv || !self->priv->ctx) {
    return;
  }
  slsContext *ctx = self->priv->ctx;
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(ctx->window);

}

void sls_mouse_button_callback(GLFWwindow *win,
                               int button,
                               int action,
                               int mods)
{

}
