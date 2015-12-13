//
// Created by Steven on 4/25/15.
//

#include "../src/dangerengine.h"
#include <kazmath/kazmath.h>
#include <assert.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif //__EMSCRIPTEN__


#if 1

extern int earth_main(int *argcr, char **argv);

extern int render_demo_main(int *argc, char **argv);


#else
#   include <windows.h>
#endif

static inline void setup()
{
#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__

};


int main(int argc, char *argv[])
{

  setup();

  return render_demo_main(&argc, argv);

}





