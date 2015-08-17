//
// Created by Steven on 8/17/15.
//

#ifndef DANGERENGINE_SLSSPRITE_H
#define DANGERENGINE_SLSSPRITE_H

#include "slsshader.h"
#include "slsmesh.h"

#include "slstexture.h"
#include "../math/slsTransform2D.h"


typedef struct slsSprite slsSprite;
typedef struct slsSprite_p slsSprite_p;

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

  slsSprite_p *priv;
  void *data;
};


slsSprite *sls_sprite_new(slsTexture *tex,
                          float const sprite_uvs[8],
                          slsTransform2D const *transform);

slsSprite const *sls_sprite_class();


#endif //DANGERENGINE_SLSSPRITE_H
