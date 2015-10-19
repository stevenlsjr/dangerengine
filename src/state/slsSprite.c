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
    SLS_COMPONENT_TEXTURE |
    SLS_COMPONENT_KINETIC |
    SLS_COMPONENT_BOUNDED |
    SLS_COMPONENT_MATERIAL |
    SLS_COMPONENT_MESH;

bool sls_is_spriteentity(slsEntity *self)
{
  return ((self->component_mask & sprite_mask) == sprite_mask) &&
         self->mesh && self->mesh->_vertices &&
         sls_array_length(self->mesh->_vertices) == 4;
}

slsSprite *sls_init_sprite(slsEntity *self, apr_pool_t *parent_pool, char const *name, slsTexture *tex,
                           slsShader *shader)
{
  self = sls_entity_init(self, parent_pool, sprite_mask, name);

  sls_checkmem(self);

  self->mesh = sls_mesh_square();
  self->mesh_is_owned = true;

  self->shader = shader;
  if (!shader) {
    self->component_mask &= ~SLS_COMPONENT_MATERIAL;
  }

  self->texture = tex;

  sls_msg(self->mesh, bind, self->shader);
  sls_msg(self->texture, set_program, self->shader->program);

  return self;

  error:
  if (self) {
    return sls_entity_class()->dtor(self);
  } else { return NULL; }
}

void sls_sprite_set_color(slsEntity *self, kmVec4 color, bool update_vbo)
{
  glBindBuffer(GL_ARRAY_BUFFER, self->mesh->vbo);
  if (!sls_is_spriteentity(self)) { return; }
  if (self->mesh) {
    size_t n_verts = sls_array_length(self->mesh->_vertices);

    for (int i=0; i<n_verts; ++i) {
      slsVertex *v = sls_array_get(self->mesh->_vertices, (size_t)i);
      size_t offset = i * sizeof(slsVertex) + offsetof(slsVertex, color);
      memcpy(v->color, &color, sizeof(float) * 4);

      glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float) * 4, v->color);
    }

  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void sls_sprite_get_color(slsEntity *self, kmVec4 *color_out)
{
  if (!sls_is_spriteentity(self)) { return; }
  if (color_out) {
    slsVertex *v = sls_array_get(self->mesh->_vertices, 0);
    memcpy(color_out, v->color, sizeof(float) * 4);
  }
}

void sls_sprite_set_uvbox(slsEntity *self, slsVRect const *box, bool update_vbo)
{
  if (!sls_is_spriteentity(self)) { return; }

}

void sls_sprite_get_uvbox(slsEntity *self, slsVRect *box_out)
{
  if (!sls_is_spriteentity(self)) { return; }

}
