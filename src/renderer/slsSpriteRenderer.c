//
// Created by Steven on 7/22/15.
//

/**
 * @file slsSpriteRenderer.c
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

#include "slsSpriteRenderer.h"
#include "slsSprite.h"

#include <assert.h>
#include <string.h>

static void sls_sprite_freefn(void *sprite)
{
  if (sprite) {
    slsSprite *sprite1 = sprite;
    free(sprite1->dtor(sprite1));
  }
}

slsSpriteBatch *sls_spritebatch_init(slsSpriteBatch *self)
{

  if (!self) { return NULL; }

  self->sprites = *sls_ptrarray_init(&self->sprites, ((void *[]){}), 0, sls_sprite_freefn);


  return self;
  error:
    return sls_spritebatch_dtor(self);
}


slsSpriteBatch *sls_spritebatch_dtor(slsSpriteBatch *self)
{

  sls_ptrarray_dtor(&self->sprites);

  return self;
error:
  return self;
}


void sls_spritebatch_display(slsSpriteBatch *self, double dt)
{

}
