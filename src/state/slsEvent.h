/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/20/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSEVENT_H
#define DANGERENGINE_SLSEVENT_H


#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct slsContext slsContext;

typedef struct slsEventClass slsEventClass;
struct slsEventClass {
  uint32_t sdl_type;

  char *name;
  void *static_data;
  bool is_registered;
};

slsEventClass *sls_eventclass_register(slsEventClass *klass);


#endif //DANGERENGINE_SLSEVENT_H
