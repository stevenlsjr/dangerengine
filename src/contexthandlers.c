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
#include "sls-commonlibs.h"

static pthread_mutex_t sls_active_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool sls_active_flag = false;

static inline bool
sls_init_sdl(uint32_t sdl_flags)
{
  int sdl = SDL_Init(sdl_flags);
  sls_log_info("sdl created: %i", sdl);
  return sdl == 0;
}

static inline bool
sls_init_img(int32_t img_flags)
{
  int32_t img = IMG_Init(img_flags);
  return (img & img_flags) == img_flags;
}

bool
sls_init(void)
{
  sls_check(!sls_active_flag, "runtime is already active!");

  uint32_t sdl_flags = SDL_INIT_EVERYTHING;
  int32_t img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

  sls_check(sls_init_sdl(sdl_flags), "sdl creation failed %s", SDL_GetError());
  sls_check(sls_init_img(img_flags), "img creation failed %s", IMG_GetError());

  sls_active_flag = true;

  atexit(sls_terminate);

  return true;
error : {
  assert(0);
  exit(EXIT_FAILURE);
  return false;
}
}

void
sls_terminate(void)
{
  if (!sls_active_flag) {
    return;
  }

  if (SDL_WasInit(0)) {
    SDL_Quit();
  }
  IMG_Quit();

  sls_active_flag = false;
}

bool
sls_is_active(void)
{
  return sls_active_flag;
}
