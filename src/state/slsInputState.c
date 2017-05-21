/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#include "slsInputState.h"
#include <SDL2/SDL.h>
#include <math/math-types.h>

struct slsInputState {
  slsIPoint last_mousepos;
  int last_mousemask;
};

slsInputState* sls_inputstate_new()
{
  slsInputState* self = calloc(1, sizeof(slsInputState));
  sls_checkmem(self);
  return self;
error:
  if (self) {
    sls_inputstate_delete(self);
  }
  return NULL;
}

void sls_inputstate_delete(slsInputState* self)
{
  sls_check(self, "slsInputState must be non-null");

  free(self);
  return;
error:
  return;
}

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
