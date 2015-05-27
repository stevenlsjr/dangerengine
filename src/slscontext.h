//
// Created by Steven on 4/25/15.
//

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include <GLFW/glfw3.h>
#include "slsutils.h"

typedef struct slsContext slsContext;
typedef struct slsContext_p slsContext_p;

struct slsContext {
  slsContext * (*init)(slsContext *self,
                       char const *caption,
                       size_t width, size_t height);
  void (*dtor)(slsContext *self);

  void (*run)(slsContext *self);

  void (*iter) (slsContext *self);

  void (*resize)(slsContext *self, size_t x, size_t y);

  void (*update)(slsContext *self, double dt);

  void (*display)(slsContext *self, double dt);



  GLFWwindow *window;
  slsBool is_running;
  clock_t interval;
  slsContext_p *priv;
};

slsContext const *sls_context_class();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);

#endif //DANGERENGINE_SLSCONTEXT_H
