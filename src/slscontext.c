/**
 * @file slscontext.c
 * @brief
 *
 **/

#include "contexthandlers.h"
#include "renderer/slsrender.h"
#include "math/math-types.h"
#include "renderer/slssprite.h"


#define SLS_TICKS_PER_SEC 1000
#ifdef GLAD_DEBUG


struct slsContext_p {
  uint64_t last;
  uint64_t ticks_since_draw;
  slsIPoint last_size;
  slsRendererGL renderer;
  // demo resources

  slsShader shader;

  slsSprite sprite;
};

static void pre_gl_call(char const *name, void *glfunc, int len_args, ...)
{
}

static void post_gl_call(char const *name, void *glfunc, int len_args, ...)
{
  GLenum err = GL_NO_ERROR;
  err = glad_glGetError();
  if (err != GL_NO_ERROR) {
    char symbol[255];
    switch (err){
      case GL_INVALID_VALUE:
        strcpy(symbol, "GL_INVALID_VALUE");
        break;
      case GL_INVALID_OPERATION:
        strcpy(symbol, "GL_INVALID_OPERATION");
        break;
      case GL_INVALID_ENUM:
        strcpy(symbol, "GL_INVALID_ENUM");
        break;
      default:
        strcpy(symbol, "???");

    }
    sls_log_err("gl error 0x%x(%s): %s", err, symbol, name);
  }
}

static void debug_message_cb(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei length,
                             const GLchar *message,
                             const void *user_param)
{
  char const *severity_strs[] = {"GL_DEBUG_SEVERITY_HIGH",
                                 "GL_DEBUG_SEVERITY_MEDIUM",
                                 "GL_DEBUG_SEVERITY_LOW"};
  size_t severity_select;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      severity_select = 0;
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      severity_select = 1;
      break;
    default:
      severity_select = 2;
  }

  char symbol[255] = "";


  sls_log_info("%s, source 0x%x , type 0x%x: %s",
               severity_strs[severity_select],
               source,
               type,
               message);
}

#endif


/*----------------------------------------*
 * slsContext static prototype
 *----------------------------------------*/
static const slsContext sls_context_proto = {

    .is_running = false,
    .interval = 1000 / 60,
    .priv = NULL,
    .window = NULL,
    .data = NULL,
};

/*----------------------------------------*
 * slsContext class functions
 *----------------------------------------*/

slsContext const *sls_context_prototype()
{
  return &sls_context_proto;
}

slsContext *sls_context_new(char const *caption, size_t width, size_t height)
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

slsContext *sls_context_init(slsContext *self,
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

  // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG |
                      SDL_GL_CONTEXT_DEBUG_FLAG);

  for (int i = 0; !self->gl_context && i < sizeof(versions) / sizeof(*versions);
       ++i) {

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
  int major, minor;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
  if (GLAD_GL_ARB_debug_output) {
    glDebugMessageCallback(debug_message_cb, NULL);
  }
  glad_set_pre_callback(pre_gl_call);
  glad_set_post_callback(post_gl_call);
#endif

  // allocate and initialize private members

  self->priv = calloc(1, sizeof(slsContext_p));
  sls_checkmem(self->priv);
  sls_renderer_init(&self->priv->renderer, (int)width, (int)height);

  return self;

error:
  sls_log_err("sdl error: %s", SDL_GetError());
  return sls_context_dtor(self);
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

  while (self->is_running) {
    sls_context_iter(self);
  }

  sls_context_teardown(self);
}

slsContext *sls_context_dtor(slsContext *self)
{
  if (self->window) {
    SDL_DestroyWindow(self->window);
  }
  // free private members
  if (self->priv) {
    sls_renderer_dtor(&self->priv->renderer);
    free(self->priv);
  }
  return self;
}

#ifdef __EMSCRIPTEN__
static void em_main_loop_iter(void *context_self){
  sls_context_iter((slsContext*)context_self);
}
#endif // __EMSCRIPTEN__

void sls_context_iter(slsContext *self)
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
    sls_context_update(self, dt);


    sls_context_display(self, dt);

    sls_context_pollevents(self);
    self->frame_n++;
  }
}

void sls_context_resize(slsContext *self, int x, int y)
{
  glViewport(0, 0, (int) x, (int) y);

  if (self->priv) {
    sls_renderer_resize(&self->priv->renderer, x, y);
  }
}

void sls_context_update(slsContext *self, double dt)
{
}

void sls_context_display(slsContext *self, double dt)
{
  glClearColor(0.0, 1.0, 0.0, 1.0);
  slsRendererGL *r = &self->priv->renderer;
  glUseProgram(self->priv->shader.program);
  sls_renderer_clear(r);
  sls_sprite_draw(&self->priv->sprite, r);

  sls_renderer_swap(r, self);


}

void sls_context_setup(slsContext *self)
{
  if (!self->priv) {
    exit(EXIT_FAILURE);
  }

  slsContext_p *priv = self->priv;

  sls_context_setupstate(self);

  sls_log_info("openGL version %s", glGetString(GL_VERSION));


  int x, y;
  SDL_GetWindowSize(self->window, &x, &y);

  sls_context_resize(self, x * 2, y * 2);

  sls_checkmem(
      sls_shader_from_sources(&self->priv->shader,
                              SLS_DEFAULT_VS, SLS_DEFAULT_FS,
                              SLS_DEFAULT_UNIFORMS));

  // setup sprite
  sls_checkmem(sls_sprite_init(&self->priv->sprite, SLS_DEFAULT_TRANSFORM));

  return;
error:
  sls_log_err("failure!");
}

void sls_context_setupstate(slsContext *self)
{
}

void sls_context_pollevents(slsContext *self)
{

  SDL_Event e;

  if (!self->window) {
    abort();
  }

  if (self->is_running) {
    while (SDL_PollEvent(&e)) {
      sls_context_handle_event(self, &e);
    }
  }
}

static inline void _sls_context_windowevent(slsContext *self,
                                            SDL_WindowEvent const *we)
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

void sls_context_handle_event(slsContext *self, SDL_Event const *e)
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
}

void sls_context_teardown(slsContext *self)
{
  sls_sprite_dtor(&self->priv->sprite);
  sls_shader_dtor(&self->priv->shader);
}


int sls_get_glversion()
{
  int major = 0, minor = 0, major_mul = 100, minor_mul = 10, full;

  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  full = (major * major_mul) + (minor * minor_mul);
  return full;
}

