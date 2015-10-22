/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/20/15, Steven
 * 
 **/
#include "slsEvent.h"

slsEventClass *sls_eventclass_register(slsEventClass *klass)
{
  klass->sdl_type = SDL_RegisterEvents(1);
  return klass;
}
