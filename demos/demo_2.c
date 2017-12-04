/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include <dangerengine.h>

#ifndef __EMSCRIPTEN__

int main(int argc, char** argv)
{
  slsContext* ctx = sls_context_new("hello world!", 640, 640);

  if (ctx) {
    sls_context_run(ctx);
  }
  free(sls_context_dtor(ctx));
  return 0;
}

#else

#include <SDL2/SDL.h>
#include <emscripten.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Emscripten entry point
 **/

static int n = 0;

static void em_loop_fn(void)
{
  char buff[100];
  fgets(buff, 100, stdin);
  buff[99] = '\0';

  printf("hello %i %s!!!\n", n, buff);
  
  if(n++>= 100){
    emscripten_cancel_main_loop();
  }
}

int main(int argc, char** argv)
{
  int res = SDL_Init(SDL_INIT_VIDEO);
  emscripten_set_main_loop(em_loop_fn, 50, false);

  return 0;
}
#endif // !__EMSCRIPTEN__
