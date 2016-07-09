/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 5/31/16, Steven
 * 
 **/
#include "slsDebugDraw.h"

#define SLS_DRAWBATCH_INIT_SIZE 0x100;

void sls_drawbatch_render(slsDrawBatch *self, slsContext *ctx)
{

}

void sls_ddraw_rect(slsDrawBatch *self, slsFRect const *bounds)
{

}

slsDrawBatch *sls_drawbatch_init(slsDrawBatch *self)
{

  self->n_allocated = SLS_DRAWBATCH_INIT_SIZE;

  self->ibos = self->vbos = NULL;
  self->ibos = calloc(self->n_allocated, sizeof(GLuint));
  self->vbos = calloc(self->n_allocated, sizeof(GLuint));
  sls_checkmem(self->ibos);
  sls_checkmem(self->vbos);

  return self;

  error:
  return NULL;
}

slsDrawBatch *sls_drawbatch_dtor(slsDrawBatch *self)
{
  if (self->vbos) { free(self->vbos); }
  if (self->ibos) { free(self->ibos); }
  *self = (slsDrawBatch){};
  return self;
}

