/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/20/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSTTFTEXT_H
#define DANGERENGINE_SLSTTFTEXT_H

#include <stdlib.h>
#include <apr_pools.h>
#include <slsutils.h>
#include "slsshader.h"

typedef struct slsTexture slsTexture;
typedef struct slsMesh slsMesh;
typedef struct slsTTFText slsTTFText;

typedef struct slsTTFText_p slsTTFText_p;

struct slsTTFText {
  slsTexture *tex;
  struct {
    slsMesh **data;
    size_t n_meshes;
  } meshes;

  slsTTFText_p *priv;

  apr_pool_t *pool;
};

slsTTFText *sls_ttftext_init(slsTTFText *self,
                             apr_pool_t *parent_pool,
                             char const *font,
                             char const *text,
                             slsShader *shader) SLS_NONNULL(1, 2, 3, 4, 5);

slsTTFText *sls_ttftext_dtor(slsTTFText *self);



#endif //DANGERENGINE_SLSTTFTEXT_H
