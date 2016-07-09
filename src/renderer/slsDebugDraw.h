/**
 * @file ${FILE}
 * @brief simple quasi-immediate mode
 * renderer for debug use
 * @license ${LICENSE}
 * Copyright (c) 5/31/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSDEBUGDRAW_H
#define DANGERENGINE_SLSDEBUGDRAW_H

#include <slscontext.h>

typedef struct slsDrawBatch slsDrawBatch;

struct slsDrawBatch {
  GLuint *vbos;
  GLuint *ibos;

  size_t n_allocated;
  size_t n_used;


};

slsDrawBatch *sls_drawbatch_init(slsDrawBatch *self) SLS_NONNULL(1);
slsDrawBatch *sls_drawbatch_dtor(slsDrawBatch *self) SLS_NONNULL(1);


void sls_ddraw_rect(slsDrawBatch *self,
                    slsFRect const *bounds);

void sls_drawbatch_render(slsDrawBatch *self, slsContext *ctx);


#endif //DANGERENGINE_SLSDEBUGDRAW_H
