//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_RENDER_DEMO_H
#define DANGERENGINE_RENDER_DEMO_H

#include <math/slsMatrixStack.h>
#include <kazmath/GL/mat4stack.h>
#include "../src/dangerengine.h"

#define DEMO_MAX_MODELS 20

typedef struct demoData {
  GLuint program;



  slsShader *shader;

  slsTexture *tank_tex;
  slsTexture *barrel_tex;
  slsTexture *grass_tex;

  kmMat4 view_matrix;

  slsMatrixStack model_view;

  slsIPoint mouse_motion;
  slsIPoint mouse_p;

  slsIPoint camera_move_input;

  slsEntity *tank;
  slsEntity *grass;


  slsCamera camera;

} demoData;

int render_demo_main(int *argc, char **argv);

void demo_context_setup(slsContext *self) SLS_NONNULL(1);

void demo_context_teardown(slsContext *self) SLS_NONNULL(1);


void demo_context_display(slsContext *self, double dt);
void demo_context_update(slsContext *self, double dt);


void demo_setup_textures(slsContext *pContext) SLS_NONNULL(1);

void demo_setup_shaders(slsContext *self) SLS_NONNULL(1);

void demo_update_uniforms(slsContext *self, double dt);


void demo_setup_scene(slsContext *self) SLS_NONNULL(1);



void demo_handle_event(slsContext *self, const SDL_Event *e) SLS_NONNULL(1);

int render_demo_main(int *argc, char **argv);

#endif //DANGERENGINE_RENDER_DEMO_H
