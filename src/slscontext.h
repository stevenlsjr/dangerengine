//
// Created by Steven on 4/25/15.
//

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include <GLFW/glfw3.h>
#include "slsutils.h"

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
                       size_t width, size_t height);

  /**
   * @brief destructor method
   * @detail finalizes object and frees it from memory
   */
  void (*dtor)(slsContext *self);



  /**
   * @brief sets up opengl and rendering context
   * @detail executed at the begining of main loop
   * must be called to use openGL functions with this context
   */
  void (*setup)(slsContext *self);

  /**
   *  @brief tears down context
   */
  void (*teardown)(slsContext *self);


  /**
   * @brief sets up run loop
   * @detail sets up context, executes main loop, and tears down context at end
   */
  void (*run)(slsContext *self);

  /**
   * @brief performs a single iteration of the mainloop
   */
  void (*iter) (slsContext *self);

  /**
   * @brief callback method for window resize
   */
  void (*resize)(slsContext *self, int x, int y);

  /**
   * @brief update method
   */
  void (*update)(slsContext *self, double dt);

  /**
   * @brief performs all drawing operations
   */
  void (*display)(slsContext *self, double dt);



  GLFWwindow *window;
  slsBool is_running;
  clock_t interval;
  slsContext_p *priv;
};

slsContext const *sls_context_class();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);


/**
 * @brief sets emscripten mainloop for context execution
 */
void sls_emscripten_loop(void *vctx);

#endif //DANGERENGINE_SLSCONTEXT_H
