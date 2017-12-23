/**
 * @file slscontext.h
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
**/

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include <sls-commonlibs.h>
#include "slsutils.h"

typedef struct slsContext slsContext;
typedef struct slsContext_p slsContext_p;

/**
 * @brief context object for glfw renderer
 */
struct slsContext {
  SDL_Window* window;
  SDL_GLContext gl_context;


  long frame_n;

  bool is_running;
  uint64_t interval;
  slsContext_p* priv;

  void* data;
};

slsContext const*
sls_context_prototype();

slsContext*
sls_context_new(char const* caption, size_t width, size_t height);

slsContext*
sls_context_dtor(slsContext* self);

static inline void
sls_context_delete(slsContext* ctx)
{
  ctx = sls_context_dtor(ctx);
  if (ctx) {
    free(ctx);
  }
}

int
sls_get_glversion();

void
sls_context_pollevents(slsContext* self) SLS_NONNULL(1);

/*----------------------------------------*
 * slsContext default method prototypes
 *----------------------------------------*/
void
sls_context_run(slsContext* self) SLS_NONNULL(1);

void
sls_context_iter(slsContext* self) SLS_NONNULL(1);

void
sls_context_resize(slsContext* self, int x, int y) SLS_NONNULL(1);

void
sls_context_update(slsContext* self, double dt) SLS_NONNULL(1);

void
sls_context_display(slsContext* self, double dt) SLS_NONNULL(1);

slsContext*
sls_context_init(slsContext* self,
                 char const* caption,
                 size_t width,
                 size_t height) SLS_NONNULL(1);

void
sls_context_setup(slsContext* self) SLS_NONNULL(1);

void
sls_context_teardown(slsContext* self) SLS_NONNULL(1);

slsContext*
sls_context_dtor(slsContext* self) SLS_NONNULL(1);

void
sls_context_handle_event(slsContext* self, SDL_Event const* e);

void
sls_context_setupstate(slsContext* pContext) SLS_NONNULL(1);

#endif // DANGERENGINE_SLSCONTEXT_H
