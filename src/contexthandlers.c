/**
 * @file contexthandlers.c
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
