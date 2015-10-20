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

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <signal.h>

#include "contexthandlers.h"
#include "slsutils.h"
#include "sls-imagelib.h"

static pthread_mutex_t sls_active_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
static slsBool sls_active_flag = SLS_FALSE;



static inline bool sls_init_sdl(uint32_t sdl_flags)
{
  int sdl = SDL_Init(sdl_flags);
  sls_log_info("sdl created: %i", sdl);
  return sdl == 0;
}

static inline bool sls_init_img(int32_t img_flags)
{
  int32_t img = IMG_Init(img_flags);
  return (img&img_flags) == img_flags;
}

static inline bool sls_init_ttf(int32_t ttf_flags)
{
  int ttf = TTF_Init();

  return ttf !=-1;
}


bool sls_init(void)
{

  sls_check(!sls_active_flag, "runtime is already active!");


  uint32_t sdl_flags = SDL_INIT_EVERYTHING;
  int32_t img_flags = IMG_INIT_JPG |
                       IMG_INIT_PNG |
                       IMG_INIT_TIF;



  sls_check(sls_init_sdl(sdl_flags), "sdl creation failed %s", SDL_GetError());
  sls_check(sls_init_img(img_flags), "img creation failed %s", IMG_GetError());
  sls_check(sls_init_ttf(0), "ttf creation failed %s", TTF_GetError());

  sls_check(apr_initialize() == APR_SUCCESS, "apr failed to init");



  sls_active_flag = SLS_TRUE;

  sls_setup_errstack();

  atexit(sls_terminate);

  return true;
  error:
  {
    assert(0);
    exit(EXIT_FAILURE);
    return false;
  }

}

void sls_terminate(void)
{
  if (!sls_active_flag) {
    return;
  }

  SDL_Quit();
  IMG_Quit();

  apr_terminate();

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
