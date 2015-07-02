//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_RENDER_DEMO_H
#define DANGERENGINE_RENDER_DEMO_H

#include "../src/dangerengine.h"

typedef struct demoData {
  GLuint program;

  GLuint tex;

  slsMesh *mesh;
  slsModel *model;

  struct {
    GLint time_;
    GLint projection;
    GLint model_view;
    GLint normal_mat;
    GLint tex_sample;
  } uniforms;

  kmMat4 model_view;
} demoData;

int render_demo_main(int *argc, char **argv);

#endif //DANGERENGINE_RENDER_DEMO_H
