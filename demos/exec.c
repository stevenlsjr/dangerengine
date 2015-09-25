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

#include "render_demo.h"

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
  

  return render_demo_main(&argc, argv);

}



