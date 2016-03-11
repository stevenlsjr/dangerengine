/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/25/15, Steven
 *
 * 
 **/

#include <unistd.h>

#include <dlfcn.h>
#include "paths.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <dangerengine.h>


// TODO (Steve) setup plugin system to make unified client!

char **find_plugin_args(int argc, char **argv)
{
  char **arr = NULL;
  arr = calloc(argc + 1, sizeof(char *));
  char **itor = arr;

  char const *plugin_flag = "-plug";

  for (int i = 1; i < argc - 1; ++i) {

    if (strncmp(plugin_flag, argv[i], 1080) == 0) {
      *itor = argv[i + 1];
      ++itor;
      ++i;

    }

  }

  arr[argc] = NULL;
  return arr;

}

int main(int argc, char **argv)
{

  slsContext _ctxval;
  slsContext *ctx = sls_context_new("client", 640, 480);

  char **plugins = find_plugin_args(argc, argv);
  if (plugins) {
    for (char **iter = plugins; *iter != NULL; iter++) {
      printf("%s\n", *iter);
    }
    free(plugins);
  }


  sls_msg(ctx, run);

  sls_context_delete(ctx);


  return 0;
}