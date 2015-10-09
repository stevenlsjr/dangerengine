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


/*================================
 * IMPLEMENTATIONS
 *================================*/

static slsComponentMask sprite_mask =
    SLS_COMPONENT_STATEACCESS |
    SLS_COMPONENT_TEXTURE |
    SLS_COMPONENT_KINETIC |
    SLS_COMPONENT_BOUNDED |
    SLS_COMPONENT_MATERIAL |
    SLS_COMPONENT_MESH;

bool sls_is_spriteentity(slsEntity *entity)
{
  return false;
}

slsSprite *sls_init_sprite(slsEntity *self,
                           slsAppState *state, apr_pool_t *parent_pool, char const *name,
                           slsTexture *tex,
                           slsShader *shader)
{
  self = sls_entity_init(self, parent_pool, sprite_mask, name);

  sls_checkmem(self);

  self->mesh = sls_mesh_square();
  self->mesh_is_owned = true;

  self->material = shader;
  if (!shader) {
    self->component_mask &= ~SLS_COMPONENT_MATERIAL;
  }

  sls_msg(self->mesh, bind, self->material);


  return self;

  error:
  if (self) {
    return sls_entity_class()->dtor(self);
  } else { return NULL; }
}
