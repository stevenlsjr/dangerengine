/**
 * @file
 * @brief
 *
**/
#ifndef SLS_SPRITE_H
#define SLS_SPRITE_H

#include "../sls-commonlibs.h"

typedef struct slsSprite slsSprite;


struct slsSprite {
  GLuint ibo;
  GLuint vbo;
  GLuint vao;

  void *texture;
};

slsSprite *sls_sprite_init(slsSprite *self);
slsSprite *sls_sprite_dtor(slsSprite *self);

#endif // SLS_SPRITE_H