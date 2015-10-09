/**
 * @file ${FILE}
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
 * either expressed or implied, of ${ORGANIZATION_NAME}. **/
#include <CoreGraphics/CoreGraphics.h>
#include "slsMatrixStack.h"

#define SLS_MATRIXSTACK_RESIZE_MULTIPLYER 1.5
#define SLS_MATRIXSTACK_DEFAULTSIZE 16


slsMatrixStack *sls_matrix_stack_init(slsMatrixStack *self,
                                      size_t initial_size)
{

  if (initial_size < 2) {
    initial_size = SLS_MATRIXSTACK_DEFAULTSIZE;
  }
  if (self->matrices) {
    sls_matrix_stack_dtor(self);
  }

  *self = (slsMatrixStack) {
      .matrices=calloc(initial_size, sizeof(kmMat4)),
      .n_matrices=0,
      .n_alloced=initial_size};

  sls_checkmem(self->matrices);

  return self;

  error:
  return NULL;
}

slsMatrixStack *sls_matrix_stack_dtor(slsMatrixStack *self)
{
  if (self->matrices) {
    free(self->matrices);
  }
  *self = (slsMatrixStack) {.matrices=NULL, .n_alloced=0, .n_matrices = 0};

  return self;
}

kmMat4 *sls_matrix_stack_pop(slsMatrixStack *self,
                             kmMat4 *out)
{
  sls_check(self->matrices, "matrix array is NULL");

  *out = self->matrices[self->n_matrices];
  --self->n_matrices;

  return out;

  error:
  return NULL;
}

void sls_matrix_stack_push(slsMatrixStack *self, kmMat4 const *in)
{


  if (self->n_matrices == self->n_alloced) {
    size_t new_size = (size_t) (self->n_alloced * SLS_MATRIXSTACK_RESIZE_MULTIPLYER);

    sls_matrix_stack_reserve(self, new_size);

  } else if (self->n_matrices >= self->n_alloced) {
    sls_check(0, "number of matrices exceedes allocated size");
  }

  self->matrices[self->n_matrices] = *in;
  self->n_matrices++;

  return;
  error:

  sls_matrix_stack_dtor(self);
  return;
}

void sls_matrix_stack_reserve(slsMatrixStack *self, size_t size)
{
  // don't resize to less than number of items
  if (size <= self->n_matrices) {return;}

  // only resize if desired size is bigger or significantly smaller than
  // current allocated size
  if (size > self->n_alloced || size < (self->n_alloced/2)) {
    self->matrices = realloc(self->matrices, size * sizeof(kmMat4));

    sls_checkmem(self->matrices);
    self->n_alloced = size;
  }

  return;
  error:
  return;

}

kmMat4 *sls_matrix_stack_peek(slsMatrixStack *self)
{
  return self->matrices + self->n_matrices - 1;
}


kmMat4 *sls_matrix_stack_mutpeek(slsMatrixStack *self)
{
  return self->matrices + self->n_matrices;
}

slsMatrixStack *sls_glmat_defaultinit(slsMatrixStack *self)
{
  const size_t init_size = 8;
  self = sls_matrix_stack_init(self, init_size);

  kmMat4 ident;
  kmMat4Identity(&ident);

  return self;
}


void sls_glmat_translate(slsMatrixStack *self, kmVec3 translation)
{
  kmMat4 *top = sls_matrix_stack_peek(self);

  kmMat4 m, tmp;
  tmp = *top;
  kmMat4Translation(&m, translation.x, translation.y, translation.z);

  kmMat4Multiply(top, &m, &tmp);

}

void sls_glmat_scale(slsMatrixStack *self, kmVec3 scaling)
{
  kmMat4 *top = sls_matrix_stack_peek(self);
  kmMat4 tmp = *top;
  kmMat4 m;
  kmMat4Scaling(&m, scaling.x, scaling.y, scaling.z);

  kmMat4Multiply(top, &m, &tmp);
}

void sls_glmat_push(slsMatrixStack *self)
{
  sls_matrix_stack_push(self, sls_matrix_stack_peek(self));

}

void sls_glmat_identity(slsMatrixStack *self)
{
  kmMat4 *top = self->matrices + self->n_matrices;

  kmMat4Identity(top);
}

void sls_glmat_bind_top(slsMatrixStack *self,
                        GLuint program,
                        GLuint uniform)
{

  glUseProgram(program);


  kmMat4 *top = sls_matrix_stack_peek(self);
  glUniformMatrix4fv(uniform, 1, GL_FALSE, top->mat);


  glUseProgram(0);
}

void sls_glmat_reset(slsMatrixStack *self)
{
  self->n_matrices = 1;
  kmMat4Identity(self->matrices);

  sls_matrix_stack_reserve(self, SLS_MATRIXSTACK_DEFAULTSIZE);
}
