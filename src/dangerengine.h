//
// Created by Steven on 4/25/15.
//

#ifndef DANGERENGINE_DANGERENGINE_H
#define DANGERENGINE_DANGERENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sls-gl.h"

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


#include "renderer/shaderutils.h"
#include "renderer/slsmesh.h"
#include "renderer/slsmodel.h"
#include "renderer/slstexture.h"
#include "renderer/slsshader.h"
#include "renderer/slsSprite.h"
#include "renderer/slsSpriteRenderer.h"

#include "ecs/slsEntity.h"

#include "objc/DangerengineFW.h"

#ifdef __cplusplus
}
#endif

#endif //DANGERENGINE_DANGERENGINE_H
