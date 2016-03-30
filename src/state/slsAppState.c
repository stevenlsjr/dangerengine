/**
 * @file ${FILE}
 * @brief
 *
 * Copyright (c) 10/1/15, Steven
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
 * either expressed or implied, of ${ORGANIZATION_NAME}. **/
#include <renderer/slsshader.h>
#include <math/slsCamera.h>
#include "slsAppState.h"

void sls_appstate_keyevent(slsAppState *self, SDL_KeyboardEvent const *pEvent,
                           SDL_EventType type);

void sls_appstate_mousemotion(slsAppState *self,
                              SDL_MouseMotionEvent const *pEvent);

void sls_appstate_mousebutton(slsAppState *self,
                              SDL_MouseButtonEvent const *pEvent);

slsAppState *sls_appstate_init(slsAppState *self, apr_pool_t *parent_pool)
{

  SDL_GetMouseState(&self->input.last_pos.x, &self->input.last_pos.y);

  kmMat4Identity(&self->projection);
  sls_matrix_glinit(&self->model_view);

  self->context = NULL;

  return self;

  error:
  return NULL;
}

slsAppState *sls_appstate_dtor(slsAppState *self)
{


  sls_matrix_stack_dtor(&self->model_view);


  return self;
}

void sls_appstate_mousebutton(slsAppState *self,
                              SDL_MouseButtonEvent const *event) { }

void sls_appstate_mousemotion(slsAppState *self,
                              SDL_MouseMotionEvent const *me)
{
  slsPlayerInput *inp = &self->input;
  inp->mouse_pos = (slsIPoint) {me->x, me->y};
  inp->mouse_relative.x += me->xrel;
  inp->mouse_relative.y += me->yrel;
}

void sls_appstate_keyevent(slsAppState *self, SDL_KeyboardEvent const *ke,
                           SDL_EventType type)
{
  bool is_down = (ke->state == SDL_PRESSED);

  slsPlayerInput *inp = &self->input;

  switch (ke->keysym.scancode) {
    case SDL_SCANCODE_W:
      inp->key_up = is_down;
      break;
    case SDL_SCANCODE_S:
      inp->key_down = is_down;
      break;
    case SDL_SCANCODE_A:
      inp->key_left = is_down;
      break;
    case SDL_SCANCODE_D:
      inp->key_right = is_down;
      break;
    case SDL_SCANCODE_B:
      inp->key_brake = is_down;
      break;

    case SDL_SCANCODE_UP:
      inp->aim_up = is_down;
      break;
    case SDL_SCANCODE_DOWN:
      inp->aim_down = is_down;
      break;
    case SDL_SCANCODE_LEFT:
      inp->aim_left = is_down;
      break;
    case SDL_SCANCODE_RIGHT:
      inp->aim_right = is_down;
      break;

    case SDL_SCANCODE_SPACE:
      inp->key_space = is_down;
      break;

    default:
      break;
  }
  /*
  uint8_t const *state = SDL_GetKeyboardState(NULL);
  inp->key_up = state[SDL_SCANCODE_W];
  inp->key_down = state[SDL_SCANCODE_S];
  inp->key_right = state[SDL_SCANCODE_D];
  inp->key_left = state[SDL_SCANCODE_A];
  inp->key_space = state[SDL_SCANCODE_SPACE];
  inp->key_brake = state[SDL_SCANCODE_B];

   */
}

void sls_appstate_handle_input(slsAppState *self, SDL_Event const *event)
{
  switch (event->type) {
    case SDL_KEYDOWN: {
      sls_appstate_keyevent(self, &event->key, (SDL_KEYDOWN));
      break;
    }
    case SDL_KEYUP: {
      sls_appstate_keyevent(self, &event->key, (SDL_KEYUP));
      break;
    }
    case SDL_MOUSEMOTION: {
      sls_appstate_mousemotion(self, &event->motion);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      sls_appstate_mousebutton(self, &event->button);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      sls_appstate_mousebutton(self, &event->button);
      break;
    }
    default:
      break;
  }
}

void sls_appstate_clearinput(slsAppState *self)
{
#if 0
  self->input = (slsPlayerInput) {
      .last_pos = self->input.mouse_pos,
      .mouse_vel = sls_ipoint_sub(&self->input.mouse_pos, &self->input.last_pos)
  };

#endif
}

void sls_appstate_update(slsAppState *self, double dt)
{

  slsPlayerInput *inp = &self->input;

}

void sls_appstate_display(slsAppState *self, double dt)
{
  sls_matrix_glreset(&self->model_view);
  kmMat4 *top = sls_matrix_stack_peek(&self->model_view);
  // set camera view

}

void sls_appstate_resize(slsAppState *self, int x, int y)
{
  kmMat4 frustrum;
  kmMat4Identity(&frustrum);
  if (self->active_shader) {
    char const *projection = "projection";
    GLuint pr = self->active_shader->program;

    int proj = glGetUniformLocation(pr, projection);
    if (proj >= 0) {
      glUseProgram(self->active_shader->program);

      glUniformMatrix4fv((GLuint) proj, 1, GL_FALSE, frustrum.mat);
    }

  }


  return;
  error:
  return;

}
