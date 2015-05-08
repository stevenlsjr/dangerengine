//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"
#include <assert.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif //__EMSCRIPTEN__

int sls_main()
{
    sls_sleep((size_t)(1.2 * CLOCKS_PER_SEC));
    slsContext *c = sls_context_new("window", 640, 640);

    slsMainLoop *mainloop = sls_mainloop_new(c);
    assert(c);
    assert(mainloop->priv);

    sls_msg(mainloop, run);
    sls_msg(mainloop, dtor);

    return 0;
}


int main(int argc, char *argv[])
{

#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__

  return sls_main();
}

