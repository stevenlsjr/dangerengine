//
// Created by Steven on 8/17/15.
//

/**
 * @file slsSprite.c
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea.
**/
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
