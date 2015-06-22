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
 * @brief structure handling opengl context
 * and main loop
 * @details like most objects in this library,
 * structs contain function pointers acting as
 * instance methods. The `self` parameter should
 * be a pointer to the context object containg the pointer.
 * The sls_msg macro expands this semantics from a simpler
 * faux smalltalk-style message calling syntax
 */
struct slsContext {
  /**
   * @brief context initializer
   * @details [long description]
   * 
   * @param caption [description]
   * @param width [description]
   * @param height [description]
   * @return an initialized self
   */
  slsContext * (*init)(slsContext *self,
                       char const *caption,
                       size_t width, size_t height);

  /**
   * @brief destructor
   */
  void (*dtor)(slsContext *self);

  /**
   * @brief runs the main loop
   * @details [long description]
   */

  void (*run)(slsContext *self);

  /**
   * @brief performs a single iteration of the main
   * loop
   * @details [long description]
   */
  void (*iter) (slsContext *self);

  void (*resize)(slsContext *self, size_t x, size_t y);

  void (*update)(slsContext *self, double dt);

  void (*display)(slsContext *self, double dt);


  /**
   * @brief GLFW window handle
   */
  GLFWwindow *window;

  /**
   * @brief indicator for context running status
   */
  slsBool is_running;

  /**
   * @brief fixed schedule interval for rendering
   */
  clock_t interval;

  /**
   * @brief delta time for fixed schedule
   */
  double fixed_dt;

  slsContext_p *priv;
};

slsContext const *sls_context_class();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);

#endif //DANGERENGINE_SLSCONTEXT_H
