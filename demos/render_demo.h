//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_RENDER_DEMO_H
#define DANGERENGINE_RENDER_DEMO_H

#include "../src/dangerengine.h"

#define DEMO_MAX_MODELS 20

typedef struct demoData {
  GLuint program;

  GLuint tex;


  slsMesh *mesh;

  slsModel *models[DEMO_MAX_MODELS];
  size_t n_models;

  slsTexture *tex_obj;

  slsSprite *sprite;

  kmMat4 view_matrix;


  slsIPoint mouse_motion;
  slsIPoint mouse_p;


  struct {
    GLint time_;
    GLint projection;
    GLint model_view;
    GLint normal_mat;
    GLint tex_sample;
  } uniforms;

} demoData;

int render_demo_main(int *argc, char **argv);

#endif //DANGERENGINE_RENDER_DEMO_H
