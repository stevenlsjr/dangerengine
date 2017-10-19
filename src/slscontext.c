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
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#include "contexthandlers.h"

#include "renderer/slsrender.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif // EMSCRIPTEN

#define SLS_TICKS_PER_SEC 1000
#ifdef GLAD_DEBUG

static void pre_gl_call(char const *name, void *glfunc, int len_args, ...)
{
}

static void debug_message_cb(GLenum source, GLenum type, GLuint id,
                             GLenum severity, GLsizei length, const GLchar *message, const void *user_param)
{
  char const *severity_strs[] = {"GL_DEBUG_SEVERITY_HIGH", "GL_DEBUG_SEVERITY_MEDIUM", "GL_DEBUG_SEVERITY_LOW"};
  size_t severity_select;
  switch (severity){
    case GL_DEBUG_SEVERITY_HIGH:
      severity_select = 0;
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      severity_select = 1;
      break;
    default:
      severity_select = 2;

  }

  sls_log_info("%s, source 0x%x , type 0x%x: %s", severity_strs[severity_select], source, type, message);
}

#endif


struct slsContext_p {
  uint64_t last;
  uint64_t ticks_since_draw;
  slsIPoint last_size;
  slsRendererGL renderer;
};

/*----------------------------------------*
 * slsContext static prototype
 *----------------------------------------*/
static const slsContext sls_context_proto = {

    .is_running = false,
    .interval = 1000 / 60,
    .priv = NULL,
    .window = NULL,
    .state = NULL,
    .data = NULL,
};

/*----------------------------------------*
 * slsContext class functions
 *----------------------------------------*/

slsContext const *
sls_context_prototype()
{
  return &sls_context_proto;
}

slsContext *
sls_context_new(char const *caption, size_t width, size_t height)
{

  slsContext *self = malloc(sizeof(slsContext));
  sls_checkmem(self);

  return sls_context_init(self, caption, width, height);
  error:
  sls_log_err("fatal: memory error for slsContext");
  exit(EXIT_FAILURE);
}

/*----------------------------------------*
 * slsContext instance methods
 *----------------------------------------*/

slsContext *
sls_context_init(slsContext *self,
                 char const *caption,
                 size_t width,
                 size_t height)
{

  *self = *sls_context_prototype();
  uint32_t window_flags;
  GLenum glew;

  // initialize libraries if not active
  if (!sls_is_active()) {
    bool res = sls_init();
    sls_check(res, "initialization failed!");
  }

  // create sdl window

  window_flags =
      SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
  self->window = SDL_CreateWindow(caption,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  (int) width,
                                  (int) height,
                                  window_flags);

  sls_check(self->window, "window creation failed");

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  struct gl_version {
    int major;
    int minor;
  };

  struct gl_version versions[] = {
      {4, 5},
      {4, 4},
      {4, 1},
      {3, 3},
      {3, 1}
  };

  self->gl_context = NULL;
  int context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, context_profile);

  //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG);

  for (int i = 0; !self->gl_context && i < sizeof(versions) / sizeof(*versions); ++i) {

    struct gl_version v = versions[i];
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, v.major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, v.major);
    self->gl_context = SDL_GL_CreateContext(self->window);
  }


  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    sls_log_err("failed to load glad proc loader");
    exit(EXIT_FAILURE);
  }

#ifdef GLAD_DEBUG
  glDebugMessageCallback(debug_message_cb, NULL);
#endif


  // allocate and initialize private members

  self->priv = calloc(1, sizeof(slsContext_p));
  sls_checkmem(self->priv);
  sls_renderer_init(&self->priv->renderer, width, height);

  self->state = calloc(1, sizeof(slsAppState));
  sls_checkmem(self->state &&
               sls_app_state_init(self->state, &self->priv->renderer));

  return self;

/// exception
  error:

  sls_log_err("sdl error: %s", SDL_GetError());
  return sls_context_dtor(self);;
}

void sls_context_run(slsContext *self)
{
  if (!self->priv) {
    return;
  }
  self->priv->last = SDL_GetTicks();
  self->priv->ticks_since_draw = 0;

  self->is_running = true;

  sls_context_setup(self);

  self->frame_n = 0;

  // setup render size
  int x, y, w, h;

  SDL_GetWindowSize(self->window, &w, &h);

  sls_context_resize(self, w * 2, h * 2);

  sls_context_display(self, 0.0);
  while (self->is_running) {
    sls_context_iter(self);
  }

  sls_context_teardown(self);
}

slsContext *
sls_context_dtor(slsContext *self)
{
  if (self->window) {
    SDL_DestroyWindow(self->window);
  }
  if (self->state) {
    free(sls_app_state_deinit(self->state));
  }
  // free private members
  if (self->priv) {
    sls_renderer_dtor(&self->priv->renderer);
    free(self->priv);
  }
  return self;
}

void
sls_emscripten_loop(void *vctx)
{
  slsContext *ctx = vctx;
  sls_context_iter(ctx);
}

void
sls_context_iter(slsContext *self)
{

  if (!self->priv) {
    return;
  }

  uint64_t now = SDL_GetTicks();
  slsContext_p *priv = self->priv;
  uint64_t interval = now - priv->last;

  // double true_dt = interval/ (double) SLS_TICKS_PER_SEC;

  priv->ticks_since_draw += interval;
  priv->last = now;

  if (priv->ticks_since_draw > self->interval) {
    double dt = priv->ticks_since_draw / (double) SLS_TICKS_PER_SEC;
    // sls_log_info("dt=%f", dt);

    priv->ticks_since_draw = 0;

    // update base app state before calling update callback
    sls_app_state_update(self->state, dt);
    sls_context_update(self, dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sls_context_display(self, dt);

    SDL_GL_SwapWindow(self->window);

    // reset input state after update interval

    sls_context_pollevents(self);
    self->frame_n++;
  }
}

void
sls_context_resize(slsContext *self, int x, int y)
{
  glViewport(0, 0, (int) x, (int) y);

  if (self->state) {
  }
  if (self->priv) {
    sls_renderer_resize(&self->priv->renderer, x, y);
  }
}

void
sls_context_update(slsContext *self, double dt)
{
}

void
sls_context_display(slsContext *self, double dt)
{
  sls_renderer_display(&self->priv->renderer, self->state);
}

void
sls_context_setup(slsContext *self)
{
  if (!self->priv) {
    assert(0);
    return;
  }

  slsContext_p *priv = self->priv;

  sls_context_setupstate(self);

  sls_log_info("openGL version %s", glGetString(GL_VERSION));

  // setup opengl pipeline

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_POINT_SIZE);

  glEnable(GL_POINT_SPRITE_ARB);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int x, y;
  SDL_GetWindowSize(self->window, &x, &y);

  sls_context_resize(self, x * 2, y * 2);
}

void
sls_context_setupstate(slsContext *self)
{
}

void
sls_context_pollevents(slsContext *self)
{

  SDL_Event e;

  if (!self->window) {
    assert(!"window doesn't exist");
  }

  if (self->is_running) {
    while (SDL_PollEvent(&e)) {
      sls_context_handle_event(self, &e);
    }
  }
}

static inline void
_sls_context_windowevent(slsContext *self, SDL_WindowEvent const *we)
{
  switch (we->event) {
    case SDL_WINDOWEVENT_RESIZED: {
      int w = we->data1 * 2, h = we->data2 * 2;
      sls_context_resize(self, w, h);
    }
      break;
    default:
      break;
  }
}

void
sls_context_handle_event(slsContext *self, SDL_Event const *e)
{
  switch (e->type) {
    case SDL_QUIT:
      self->is_running = false;
      break;
    case SDL_WINDOWEVENT:
      _sls_context_windowevent(self, &e->window);
      break;
    default:
      break;
  }
  // pass event to
  if (self->state) {
  }
}

void
sls_context_teardown(slsContext *self)
{
  if (self->state) {
  }
}

#ifndef __EMSCRIPTEN__

int
sls_get_glversion()
{
  int major = 0, minor = 0, major_mul = 100, minor_mul = 10, full;

  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  full = (major * major_mul) + (minor * minor_mul);
  return full;
}

#else
int
sls_get_glversion()
{
  // emscripten uses webgl 1.0.0
  const int webgl_default_version = 100;
  return webgl_default_version;
}

#endif
