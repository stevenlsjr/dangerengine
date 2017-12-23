/**
 * @file
 * @brief
 *
**/
#ifndef SLS_SPRITE_H
#define SLS_SPRITE_H

#include "../sls-commonlibs.h"
#include "slsrender.h"

typedef struct slsSprite slsSprite;
typedef struct slsTransform2D slsTransform2D;
struct slsTransform2D{
  kmVec2 translation;
  kmVec2 rotation;
  kmVec2 scale;
};

static const slsTransform2D SLS_DEFAULT_TRANSFORM = {
    .translation = {0, 0},
    .rotation= {0, 0},
    .scale = {0, 0}
};

struct slsSprite {
  GLuint ibo;
  GLuint vbo;
  GLuint vao;

  void *texture;
  slsTransform2D transform;
};

slsSprite *sls_sprite_init(slsSprite *self, slsTransform2D transform);
slsSprite *sls_sprite_dtor(slsSprite *self);

void sls_sprite_draw(slsSprite *self, slsRendererGL *renderer);

#endif // SLS_SPRITE_H