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
#include "pong_demo.h"
#include "lab2-demo.c.h"

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

  typedef int (*demo_t)(int *, char **);

  demo_t demo = lab_demo_main;

  return demo(&argc, argv);

}



