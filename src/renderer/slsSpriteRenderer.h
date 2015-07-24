//
// Created by Steven on 7/22/15.
//

#ifndef DANGERENGINE_SLSSPRITERENDERER_H
#define DANGERENGINE_SLSSPRITERENDERER_H

#include "slsshader.h"
#include "slsmesh.h"

#include "slstexture.h"

#include <data-types/ptrarray.h>

typedef struct slsSprite slsSprite;
typedef struct slsSpriteBatch slsSpriteBatch;

struct slsSprite {
  slsTexture *texture;
};

struct slsSpriteBatch {
  slsPtrArray *sprites;
};

slsSpriteBatch *sls_spritebatch_init(slsSpriteBatch *self);

slsSpriteBatch *sls_spritebatch_dtor(slsSpriteBatch *self);

#endif //DANGERENGINE_SLSSPRITERENDERER_H

