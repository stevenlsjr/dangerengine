//
// Created by Steven on 4/25/15.
//

#include "dangerengine.h"

int main(int argc, char **argv)
{
    sls_sleep((size_t)(1.2 * CLOCKS_PER_SEC));
    slsContext *c = sls_context_new("window", 640, 640);

    slsMainLoop *mainloop = sls_mainloop_new(c);

    sls_msg(mainloop, run);
    sls_msg(mainloop, dtor);

    sls_log_info("hello!");
    sls_log_warn("warning %s", "does not compute");

    return 0;

}
