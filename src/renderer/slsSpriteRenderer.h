//
// Created by Steven on 7/22/15.
//

#ifndef DANGERENGINE_SLSSPRITERENDERER_H
#define DANGERENGINE_SLSSPRITERENDERER_H


#include <data-types/ptrarray.h>
#include "../math/slsTransform2D.h"

typedef struct slsSpriteBatch slsSpriteBatch;

struct slsSpriteBatch {
  slsPtrArray *sprites;
};



slsSpriteBatch *sls_spritebatch_init(slsSpriteBatch *self);
slsSpriteBatch *sls_spritebatch_dtor(slsSpriteBatch *self);

#endif //DANGERENGINE_SLSSPRITERENDERER_H

