//
// Created by Steven on 4/25/15.
//

#include "../src/dangerengine.h"
#include "earth.h"
#include <kazmath/kazmath.h>
#include <assert.h>
#include <string.h>




//---------------------------------demo ctx callbacks---------------------------------------

void demo_resize(slsContext *self, int x, int y) SLS_NONNULL(1);

void demo_update(slsContext *self, double dt) SLS_NONNULL(1);

void demo_display(slsContext *self, double dt) SLS_NONNULL(1);

void demo_setup(slsContext *self) SLS_NONNULL(1);

void demo_teardown(slsContext *self) SLS_NONNULL(1);


void demo_handle_event(slsContext *self, SDL_Event const *e);


typedef struct DemoCtx DemoCtx;
struct DemoCtx {
  slsContext super;
  slsShader shader;
};
static DemoCtx demo = {};







//---------------------------------context method implementations---------------------------------------

void demo_setup(slsContext *self)
{
  sls_context_setup(self);

  DemoCtx *ctx = self->data;
  assert(&ctx->super == self);
}

void demo_teardown(slsContext *self)
{
  // code here
  sls_context_teardown(self);
}

void demo_resize(slsContext *self, int x, int y)
{
  sls_context_resize(self, x, y);
}

void demo_update(slsContext *self, double dt)
{
  sls_context_update(self, dt);
}

void demo_display(slsContext *self, double dt)
{
  sls_context_display(self, dt);
}




void demo_handle_event(slsContext *self, SDL_Event const *e)
{
  sls_context_handle_event(self, e);
  switch (e->type) {
    default: {
    }
  }
}



int main(int argc, char *argv[])
{

  return earth_main(&argc, argv);

}

