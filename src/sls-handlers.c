//
// Created by Steven on 5/2/15.
//

#include "sls-handlers.h"
#include "slsutils.h"

static slsBool sls_active_flag = false;
slsContext *sls_active_context = NULL;

void sls_error(int, const char *);
void sls_mouse(GLFWwindow*, int, int, int);



void sls_bind_context(slsContext *ctx)
{
  if (sls_active_context) {
    sls_unbind_context(sls_active_context);
  }
  sls_active_context = ctx;
  glfwMakeContextCurrent(ctx->window);

  return;
error:
  return;

}

void sls_unbind_context(void)
{
  if (sls_active_context) {
    glfwMakeContextCurrent(NULL);
    sls_active_context = NULL;
  }
}

bool sls_init(void)
{
  sls_check(!sls_active_flag, "runtime is already active!");
  sls_check(glfwInit(), "glfw Init failed");

  sls_active_flag = true;

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

  glfwTerminate();
  sls_active_flag = false;
}

bool sls_is_active(void)
{
  return sls_active_flag;
}
