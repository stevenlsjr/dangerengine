/**
 * @file slsmodel.h
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

#ifndef SLS_MODEL_H
#define SLS_MODEL_H

#include "../sls-gl.h"
#include <kazmath/kazmath.h>

typedef struct kmMat4 kmMat4;
typedef struct slsMesh slsMesh;
typedef struct slsModel slsModel;
typedef struct slsModel_p slsModel_p;

struct slsModel
{
  slsModel*(*init)(slsModel* self,
                   slsMesh* mesh,
                   GLuint program,
                   kmMat4 const* transform);
  void (*dtor)(slsModel* self);
  /**
  * @brief pushes current transform to the model_view matrix stack
  * 
  * @param model_view current model_view matrix.
  * @param model_view_handle shader index for model_view matrix
  * @return composition of model transform and current model_view
  */
  kmMat4 (*push_transform)(slsModel* self,
                           kmMat4 const* model_view,
                           GLuint model_view_handle);

  void (*draw)(slsModel* self,
               GLenum draw_type,
               double dt);

  slsMesh* mesh;
  GLuint program;
  kmMat4 transform;

  slsModel_p* priv;
  void* data;
};

slsModel const* sls_model_class();
slsModel* sls_model_new(slsMesh* mesh,
                        GLuint program,
                        kmMat4 const* transform);


#endif // SLS_MODEL_H


