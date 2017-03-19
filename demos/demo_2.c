/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include <dangerengine.h>
#include "renderer/geometries.h"

int main(int argc, char **argv)
{
  slsContext *ctx = sls_context_new("hello world!", 640, 640);

  if (ctx) {
    sls_context_run(ctx);
  }
  free(sls_context_dtor(ctx));
  return 0;
}
