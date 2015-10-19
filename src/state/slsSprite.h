//
// Created by Steven on 8/17/15.
//

/**
 * @file slsSprite.h
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

#ifndef DANGERENGINE_SLSSPRITE_H
#define DANGERENGINE_SLSSPRITE_H

#include <renderer/slsshader.h>
#include <renderer/slsmesh.h>

#include <renderer/slstexture.h>
#include "../math/slsTransform2D.h"
#include "slsEntity.h"


typedef struct slsEntity slsSprite;
typedef struct slsSprite_data slsSprite_data;

slsSprite *sls_init_sprite(slsEntity *self, apr_pool_t *parent_pool, char const *name, slsTexture *tex,
                           slsShader *shader);

bool sls_is_spriteentity(slsEntity *entity);

void sls_sprite_set_color(slsEntity *self, kmVec4 color, bool update_vbo);
void sls_sprite_get_color(slsEntity *self, kmVec4 *color_out);

void sls_sprite_set_uvbox(slsEntity *self, slsVRect const *box, bool update_vbo);
void sls_sprite_get_uvbox(slsEntity *self, slsVRect *box_out);

#endif //DANGERENGINE_SLSSPRITE_H
