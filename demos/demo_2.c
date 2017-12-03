/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include <dangerengine.h>

#ifndef __EMSCRIPTEN__

int main(int argc, char **argv)
{
  slsContext *ctx = sls_context_new("hello world!", 640, 640);

  if (ctx) {
    sls_context_run(ctx);
  }
  free(sls_context_dtor(ctx));
  return 0;
}


#else

#include <SDL2/SDL.h>
#include <stdlib.h>

#include <pthread.h>

static void *thread_fn(void *_)
{
  printf("hello!!! from thread\n");
  return NULL;
}

/**
 * Emscripten entry point
 **/

int main(int argc, char **argv){
  int res =  SDL_Init(SDL_INIT_VIDEO);
  #define N_THREADS 10
  pthread_t threads[N_THREADS];
  for(int i=0; i<N_THREADS; ++i){
    pthread_create(threads + i, NULL, thread_fn, NULL);
  }

  atexit(SDL_Quit);
  printf("hello world!!! %i\n", res);

  return 0;
}
#endif // !__EMSCRIPTEN__


