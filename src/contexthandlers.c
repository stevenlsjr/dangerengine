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

#include "sls-imagelib.h"

#include <SDL2/SDL.h>
#include <pthread.h>

static pthread_mutex_t sls_active_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
static slsBool sls_active_flag = SLS_FALSE;


bool sls_il_init()
{

#ifndef __EMSCRIPTEN__
  ilInit();
  //iluInit();
  //ilutInit();

#endif
  return true;
}


bool sls_init()
{

  sls_check(!sls_active_flag, "runtime is already active!");


  uint32_t sdl_flags = SDL_INIT_EVERYTHING;
  uint32_t img_flags;

  int sdl = SDL_Init(sdl_flags);
  sls_check(sdl == 0, "sdl creation failed");
  sls_log_info("sdl created: %i", sdl);

  sls_check(sls_il_init(), "devIL failed");


  sls_active_flag = SLS_TRUE;

  sls_setup_errstack();

  atexit(sls_terminate);

  return true;
  error:
  {
    sls_active_flag = SLS_FALSE;
    return false;
  }

}

void sls_terminate(void)
{
  if (!sls_active_flag) {
    return;
  }

  SDL_Quit();

  sls_teardown_errstack();

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
