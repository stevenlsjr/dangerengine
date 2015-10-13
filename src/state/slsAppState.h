/**
 * @file ${FILE}
 * @brief
 * @license FreeBSD
 *
 * Copyright (c) 10/1/15, Steven
 * All rights reserved.
 *
 **/

#ifndef DANGERENGINE_SLSAPPSTATE_H
#define DANGERENGINE_SLSAPPSTATE_H

#include <slsutils.h>
#include <apr-1/apr_hash.h>
#include <apr-1/apr_pools.h>
#include <math/math-types.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <math/slsMatrixStack.h>
#include "slsEntity.h"

typedef struct slsEntity slsEntity;
typedef struct slsAppState slsAppState;
typedef struct slsPlayerInput slsPlayerInput;

/**
 * @brief bitmap for common key input modifiers
 */
typedef enum {
  SLS_KEMODS_NONE = 0,
  SLS_KEYMOD_CTRL = 1 << 0,
  SLS_KEYMOD_CMD  = 1 << 1,
  SLS_KEYMOD_ALT  = 1 << 2,
  SLS_KEYMOD_FN   = 1 << 3
} slsKeyMods;

/**
 * @brief simple structure for tracking user input
 * in a consistent way.
 * @detail Should not describe any behavior on its own.
 * Acts as a intermediary between platform inpud API (SDL)
 * and program logic
 */
struct slsPlayerInput {
  bool key_up;
  bool key_down;
  bool key_right;
  bool key_left;

  bool key_space;


  slsKeyMods key_mods;

  slsIPoint mouse_pos;
  slsIPoint last_pos;

  slsIPoint mouse_vel;

  slsIPoint mouse_relative;

};

/**
 * @brief
 */
struct slsAppState {

  apr_hash_t *images;
  apr_hash_t *shaders;
  apr_hash_t *textures;

  slsPlayerInput input;

  GLuint shader;

  slsCamera *active_camera;
  slsShader *active_shader;

  slsMatrixStack model_view;
  kmMat4 projection;


  slsEntity *root;

  apr_pool_t *pool;
};

/**
 * @brief clears input state. Use before repolling input.
 */
void sls_appstate_clearinput(slsAppState *state);

void sls_appstate_handle_input(slsAppState *state, SDL_Event const *event);

slsAppState *sls_appstate_init(slsAppState *self, apr_pool_t *parent_pool) SLS_NONNULL();

slsAppState *sls_appstate_dtor(slsAppState *self);

void sls_appstate_update(slsAppState *self, double dt);
void sls_appstate_display(slsAppState *self, double dt);

void sls_appstate_resize(slsAppState *self, int x, int y);


#endif //DANGERENGINE_SLSAPPSTATE_H
