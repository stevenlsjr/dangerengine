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

static inline void setup() {
#ifdef __EMSCRIPTEN__
  EM_ASM({
    var canvas = document.getElementById('canvas');
    Module.canvas = canvas;
  });
#endif //__EMSCRIPTEN__

};



int main(int argc, char *argv[]) {

  typedef int (*demo_t)(int *, char **);

  demo_t demo = NULL;

  for (int i = 1; i < argc && (argv[i] != NULL); ++i) {
    char const *s = argv[i];
    const size_t max_chars = 50;

    char const *pong_name = "pong";
    char const *render_name = "render";

    if (strncasecmp(pong_name, s, max_chars) == 0) {
      demo = pong_demo_main;
      break;

    } else if (strncasecmp(render_name, s, max_chars) == 0) {
      demo = render_demo_main;
      break;
    }
  }

  if (!demo) {
    demo = render_demo_main;
  }

  return demo(&argc, argv);

}



