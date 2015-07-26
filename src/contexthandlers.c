//
// Created by Steven on 5/2/15.
//

#include "contexthandlers.h"
#include "slsutils.h"
#include <stdlib.h>
#include <stdbool.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

static slsBool sls_active_flag = SLS_FALSE;
slsContext *sls_active_context = NULL;

void sls_error_cback(int, const char *);



void sls_mouse(GLFWwindow*, int, int, int);
void sls_window_resize(GLFWwindow *win, int x, int y);



void sls_bind_context(slsContext *ctx)
{
  sls_unbind_context();

  // window callbacks
  glfwSetWindowSizeCallback(ctx->window, sls_window_resize);

  sls_active_context = ctx;
  glfwMakeContextCurrent(ctx->window);

  return;
}

void sls_unbind_context(void)
{
  if (sls_active_context) {
    glfwMakeContextCurrent(NULL);
    sls_active_context = NULL;
  }
}


void sls_window_resize(GLFWwindow *win, int x, int y)
{
  if (sls_active_context) {
    sls_msg(sls_active_context, resize, x, y);
  }
}

void sls_mouse(GLFWwindow *wwindow, int i, int i1, int i2)
{

}

bool sls_init(void)
{
  sls_check(!sls_active_flag, "runtime is already active!");

  // setup glfw
  sls_check(glfwInit(), "glfw Init failed");

  // setup devIL
  ilInit();
  //iluInit();
  //ilutInit();
  //

  sls_active_flag = SLS_TRUE;


  // set error callback
  glfwSetErrorCallback(sls_error_cback);

  sls_setup_errstack();

  atexit(sls_terminate);

  return true;
error:
  return false;
}

void sls_terminate(void)
{
  if (!sls_active_flag) {
    return;
  }

  sls_teardown_errstack();

  glfwTerminate();
  sls_active_flag = SLS_FALSE;
}

void sls_error_cback(int i, char const *string)
{
  sls_log_err("glfw error: %s", string);
}

bool sls_is_active(void)
{
  return sls_active_flag;
}
