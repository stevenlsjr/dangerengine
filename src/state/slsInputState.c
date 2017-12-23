/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#include "slsInputState.h"
#include <sls-gl.h>
#include <math/math-types.h>

struct slsInputState {
  slsIPoint last_mousepos;
  int last_mousemask;

};

void sls_inputstate_update(slsInputState* self, double dt)
{
  // mouse update
  self->last_mousemask =
    SDL_GetMouseState(&self->last_mousepos.x, &self->last_mousepos.y);
}

slsIPoint sls_inputstate_last_mousestate(slsInputState* self, int* mask)
{
  slsIPoint result = self->last_mousepos;
  if (mask) {
    *mask = self->last_mousemask;
  }
  return result;
}
