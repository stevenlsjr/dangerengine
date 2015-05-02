//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"
#include <assert.h>

int main(int argc, char **argv)
{
    sls_sleep((size_t)(1.2 * CLOCKS_PER_SEC));
    slsContext *c = sls_context_new("window", 640, 640);

    slsMainLoop *mainloop = sls_mainloop_new(c);
    assert(c);
    assert(mainloop->priv);

    sls_msg(mainloop, run);
    sls_msg(mainloop, dtor);

}
