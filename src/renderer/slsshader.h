//
// Created by Steven on 6/23/15.
//

#ifndef DANGERENGINE_SLSSHADER_H
#define DANGERENGINE_SLSSHADER_H

#include <sls-gl.h>

typedef struct slsShader slsShader;


struct slsShader {
  slsShader *(*init)(slsShader *self, GLuint program);

  slsShader *(*dtor)(slsShader *self);
  void (*use)(slsShader *self);

  GLuint program;
  void *data;
};


#endif //DANGERENGINE_SLSSHADER_H
