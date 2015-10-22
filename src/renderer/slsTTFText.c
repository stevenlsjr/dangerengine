/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/20/15, Steven
 * 
 **/
#include "slsTTFText.h"
#include "slsshader.h"
#include <sls-commonlibs.h>

struct slsTTFText_p {
  char *text;
};

slsTTFText *sls_ttftext_init(slsTTFText *self,
                             apr_pool_t *parent_pool,
                             char const *font,
                             char const *text,
                             slsShader *shader)
{
  self->pool = NULL;
  sls_checkmem(APR_SUCCESS == apr_pool_create(&self->pool, parent_pool));

  self->priv = apr_pcalloc(self->pool, sizeof(slsTTFText_p));
  sls_checkmem(self->priv);

  self->priv->text = apr_pstrdup(self->pool, text);
  sls_checkmem(self->priv);


  return self;
  error:
  if (self) {
    sls_ttftext_dtor(self);
  }
  return self;
}

slsTTFText *sls_ttftext_dtor(slsTTFText *self)
{
  if (self->pool)

    return self;
}
