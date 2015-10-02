/**
 * @file slsMatrixStack.h
 * @brief  
 *
 * Copyright (c) 9/25/15, Steven Shea
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
 * either expressed or implied, of Steven Shea. **/
#ifndef DANGERENGINE_SLSMATRIXSTACK_H
#define DANGERENGINE_SLSMATRIXSTACK_H

#include "../sls-gl.h"
#include <kazmath/kazmath.h>
#include <stdlib.h>
#include <slsutils.h>

typedef struct slsMatrixStack slsMatrixStack;

/**
 * @brief A stack data structure for manipulating matrix
 * transformations in a way similar to immediate mode openGL.
 */
struct slsMatrixStack {
  /**
   * @brief Pointer storing the matrix array
   */
  kmMat4 *matrices;
  /**
   * @brief number of elements allocated
   */
  size_t n_alloced;
  /**
   * @brief number of elements in use
   */
  size_t n_matrices;
};

//------------------------------------------------------------------------
//--------------primitive stack operations--------------------------------
//------------------------------------------------------------------------
slsMatrixStack *sls_matrix_stack_init(slsMatrixStack *self,
                                      size_t initial_size) SLS_NONNULL(1);

slsMatrixStack *sls_matrix_stack_dtor(slsMatrixStack *self)SLS_NONNULL(1);

kmMat4 *sls_matrix_stack_pop(slsMatrixStack *self, kmMat4 *out)SLS_NONNULL(1, 2);

void sls_matrix_stack_push(slsMatrixStack *self, kmMat4 const *in) SLS_NONNULL(1, 2);


kmMat4 *sls_matrix_stack_peek(slsMatrixStack *self) SLS_NONNULL(1);

kmMat4 *sls_matrix_stack_mutpeek(slsMatrixStack *self) SLS_NONNULL(1);

void sls_matrix_stack_reserve(slsMatrixStack *self, size_t size);




//------------------------------------------------------------------------
//----------      ffp-stle matrix stack manipulations --------------------
//------------------------------------------------------------------------


slsMatrixStack *sls_glmat_defaultinit(slsMatrixStack *self)SLS_NONNULL(1);

void sls_glmat_identity(slsMatrixStack *self);

void sls_glmat_translate(slsMatrixStack *self, kmVec3 translation);

void sls_glmat_scale(slsMatrixStack *self, kmVec3 scaling);

/**
 * @brief pushes a copy of top to stack
 */
void sls_glmat_push(slsMatrixStack *self);

void sls_glmat_bind_top(slsMatrixStack *self, GLuint program, GLuint uniform);

void sls_glmat_reset(slsMatrixStack *self);

#endif //DANGERENGINE_SLSMATRIXSTACK_H
