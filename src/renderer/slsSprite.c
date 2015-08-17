//
// Created by Steven on 8/17/15.
//

#include "slsSprite.h"


#include <assert.h>
#include <string.h>

slsSprite *sls_sprite_dtor(slsSprite *self);

void sls_sprite_draw(slsSprite *self, slsMesh *mesh);

slsSprite *sls_sprite_init(slsSprite *self,
                           slsTexture *tex,
                           float const uvs[8],
                           slsTransform2D const *transform);


slsTransform2D const *sls_sprite_get_transform(slsSprite *self);

void sls_sprite_set_transform(slsSprite *self, slsTransform2D const *transform);

static slsSprite proto = {
    .init=sls_sprite_init,
    .dtor=sls_sprite_dtor,
    .draw=sls_sprite_draw,
    .set_transform=sls_sprite_set_transform,
    .get_transform=sls_sprite_get_transform

};


/*================================
 * IMPLEMENTATIONS
 *================================*/


slsSprite const *sls_sprite_class()
{

  return NULL;
}


slsSprite *sls_sprite_new(slsTexture *tex, const float sprite_uvs[8], slsTransform2D const *transform)
{
  slsSprite *self = sls_objalloc(&proto, sizeof(slsSprite));
  sls_checkmem(self);

  return self->init(self, tex, sprite_uvs, transform);
  error:
  return NULL;
}


slsSprite *sls_sprite_init(slsSprite *self,
                           slsTexture *tex,
                           float const uvs[8],
                           slsTransform2D const *transform)
{
  if (!self) { return NULL; }


  self->texture = tex;
  if (uvs) {
    memcpy(self->sprite_uvs, uvs, sizeof(self->sprite_uvs));
  } else {
    float def_uvs[] = {0.0, 0.0,
                       0.0, 1.0,
                       1.0, 1.0,
                       1.0, 0.0};
    memcpy(self->sprite_uvs,
           def_uvs, sizeof(self->sprite_uvs));
  }


  self->transform2D = transform ?
                      *transform :
                      (slsTransform2D) {};

  sls_transform2D_to_matrix(&self->transform, &self->transform2D);
  return self;
}

slsSprite *sls_sprite_dtor(slsSprite *self)
{
  if (!self) { return NULL; }

  return self;
}


void sls_sprite_draw(slsSprite *self, slsMesh *mesh)
{
  sls_log_info("not implemented");
}

slsTransform2D const *sls_sprite_get_transform(slsSprite *self)
{
  if (!self) { return NULL; }
  return &self->transform2D;
}

void sls_sprite_set_transform(slsSprite *self, slsTransform2D const *transform)
{

}
