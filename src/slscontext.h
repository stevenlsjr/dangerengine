/**
 * @file slscontext.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
**/

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include "sls-gl.h"
#include "slsutils.h"
#include "slsworkqueue.h"
#include "state/slsAppState.h"
#include <SDL2/SDL.h>

typedef struct slsContext slsContext;
typedef struct slsContext_p slsContext_p;

/**
 * @brief context object for glfw renderer
 */
struct slsContext {
  /**
   * @brief initializer
   *
   * @param caption window's caption
   * @param width window width
   * @param height window height
   *
   * @return initialized `self` object
   */
  slsContext *(*init)(slsContext *self, char const *caption, size_t width,
                      size_t height)SLS_NONNULL(1);

  /**
   * @brief destructor method
   * @detail finalizes object and frees it from memory
   */
  slsContext *(*dtor)(slsContext *self)SLS_NONNULL(1);

  /**
   * @brief sets up opengl and rendering context
   * @detail executed at the begining of main loop
   * must be called to use openGL functions with this context
   */
  void (*setup)(slsContext *self) SLS_NONNULL(1);

  /**
   *  @brief tears down context
   */
  void (*teardown)(slsContext *self) SLS_NONNULL(1);

  /**
   * @brief sets up run loop
   * @detail sets up context, executes main loop, and tears down context at end
   */
  void (*run)(slsContext *self) SLS_NONNULL(1);

  void (*handle_event)(slsContext *self, SDL_Event const *e) SLS_NONNULL(1);

  /**
   * @brief callback method for window resize
   */
  void (*resize)(slsContext *self, int x, int y) SLS_NONNULL(1);

  /**
   * @brief update method
   */
  void (*update)(slsContext *self, double dt) SLS_NONNULL(1);

  /**
   * @brief performs all drawing operations
   */
  void (*display)(slsContext *self, double dt) SLS_NONNULL(1);

  SDL_Window *window;
  SDL_GLContext gl_context;

  slsWorkScheduler queue;

  long frame_n;

  bool is_running;
  uint64_t interval;
  slsContext_p *priv;

  slsAppState *state;

  void *data;
};

slsContext const *sls_context_prototype();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);

static inline void sls_context_delete(slsContext *ctx)
{
  ctx = ctx->dtor(ctx);
  if (ctx) {
    free(ctx);
  }
}

int sls_get_glversion();

void sls_context_pollevents(slsContext *self) SLS_NONNULL(1);

/*----------------------------------------*
 * slsContext default method prototypes
 *----------------------------------------*/
void sls_context_run(slsContext *self) SLS_NONNULL(1);

void sls_context_iter(slsContext *self) SLS_NONNULL(1);

void sls_context_resize(slsContext *self, int x, int y) SLS_NONNULL(1);

void sls_context_update(slsContext *self, double dt) SLS_NONNULL(1);

void sls_context_display(slsContext *self, double dt) SLS_NONNULL(1);

slsContext *sls_context_init(slsContext *self, char const *caption,
                             size_t width, size_t height) SLS_NONNULL(1);

void sls_context_setup(slsContext *self) SLS_NONNULL(1);

void sls_context_teardown(slsContext *self) SLS_NONNULL(1);

slsContext *sls_context_dtor(slsContext *self) SLS_NONNULL(1);

void sls_context_handle_event(slsContext *self, SDL_Event const *e);

void sls_context_setupstate(slsContext *pContext) SLS_NONNULL(1);

#endif // DANGERENGINE_SLSCONTEXT_H
