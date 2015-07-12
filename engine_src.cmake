
set (DANGER_SRC
  src/dangerengine.h

  # context handling and general utilities
  src/sls-gl.h
  src/slscontext.h
  src/slscontext.c
  src/slsutils.c
  src/slsutils.h
  src/contexthandlers.c
  src/contexthandlers.h
  src/slserrcode.c
  src/slserrcode.h

  src/math/mathmacs.h
  src/math/mathmacs.c
  src/math/math-types.c
  src/math/math-types.h

  #data types
  src/data-types/callbacks.h
  src/data-types/array.h
  src/data-types/array.c
  src/data-types/hashtable.c
  src/data-types/hashtable.h

  #renderer types
  src/renderer/slsmesh.c
  src/renderer/slsmesh.h
  src/renderer/slsmodel.c
  src/renderer/slsmodel.h
  src/renderer/slstexture.c
  src/renderer/slstexture.h
  src/renderer/shaderutils.c
  src/renderer/shaderutils.h

  #resource types
  )

set (DANGER_DEMO_SRC
  demos/exec.c
  demos/render_demo.c
  demos/render_demo.h
  demos/pong_demo.c
  demos/pong_demo.h)