//
// Created by Steven on 7/22/15.
//

#ifndef DANGERENGINE_SLSSPRITERENDERER_H
#define DANGERENGINE_SLSSPRITERENDERER_H

#include "slsshader.h"
#include "slsmesh.h"

#include "slstexture.h"

#include <data-types/ptrarray.h>
#include "../math/slsTransform2D.h"

typedef struct slsSprite slsSprite;
typedef struct slsSpriteBatch slsSpriteBatch;

struct slsSprite {

  slsSprite *(*init)(slsSprite *self,
                     slsTexture *tex,
                     float const uvs[8],
                     slsTransform2D const *transform);

  slsSprite *(*dtor)(slsSprite *self);

  void (*draw)(slsSprite *self, slsMesh *mesh);

  slsTransform2D const *(*get_transform)(slsSprite *self);

  void (*set_transform)(slsSprite *self, slsTransform2D const *transform);

  slsTexture *texture;
  float sprite_uvs[8];


  slsTransform2D transform2D;
  kmMat4 transform;

  void *data;
};

struct slsSpriteBatch {
  slsPtrArray *sprites;
};


slsSprite *sls_sprite_new(slsTexture *tex,
                          float const sprite_uvs[8],
                          slsTransform2D const *transform);

slsSprite const *sls_sprite_class();

slsSpriteBatch *sls_spritebatch_init(slsSpriteBatch *self);
slsSpriteBatch *sls_spritebatch_dtor(slsSpriteBatch *self);

#endif //DANGERENGINE_SLSSPRITERENDERER_H

