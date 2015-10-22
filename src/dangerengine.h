/**
 * @file dangerengine.h
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

#ifndef DANGERENGINE_DANGERENGINE_H
#define DANGERENGINE_DANGERENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sls-gl.h"
#include "sls-imagelib.h"

#include "slscontext.h"
#include "slsutils.h"
#include "slserrcode.h"

#include "contexthandlers.h"

#include "data-types/callbacks.h"
#include "data-types/array.h"
#include "data-types/ptrarray.h"
#include "data-types/linkedlist.h"

#include "math/mathmacs.h"
#include "math/math-types.h"
#include "math/slsTransform2D.h"
#include "math/slsMatrixStack.h"


#include "renderer/shaderutils.h"
#include "renderer/slsmesh.h"
#include "renderer/slstexture.h"
#include "renderer/slsshader.h"
#include "renderer/slsTTFText.h"

#include "state/slsSprite.h"

#include "state/slsEntity.h"
#include "state/slsAppState.h"

#include "objc/DangerengineFW.h"

#ifdef __cplusplus
}
#endif

#endif //DANGERENGINE_DANGERENGINE_H
