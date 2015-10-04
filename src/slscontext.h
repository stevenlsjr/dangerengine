/**
 * @file slscontext.h
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

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include "slsutils.h"
#include "sls-gl.h"
#include <SDL2/SDL.h>
#include <state/slsAppState.h>
#include <apr-1/apr_pools.h>

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
  slsContext * (*init)(slsContext *self,
                       char const *caption,
                       size_t width, size_t height) SLS_NONNULL(1);

  /**
   * @brief destructor method
   * @detail finalizes object and frees it from memory
   */
  void (*dtor)(slsContext *self) SLS_NONNULL(1);



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

  /**
   * @brief performs a single iteration of the mainloop
   */
  void (*iter) (slsContext *self) SLS_NONNULL(1);

  void (*handle_event) (slsContext *self, SDL_Event const *e) SLS_NONNULL(1);

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


  slsBool is_running;
  clock_t interval;
  slsContext_p *priv;

  apr_pool_t *pool;

  slsAppState *state;

  void *data;
};

slsContext const *sls_context_class();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);


/**
 * @brief sets emscripten mainloop for context execution
 */
void sls_emscripten_loop(void *vctx);

int sls_get_glversion();

void sls_context_pollevents(slsContext *self) SLS_NONNULL(1);

#endif //DANGERENGINE_SLSCONTEXT_H
