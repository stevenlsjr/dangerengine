//
// Created by Steven on 4/25/15.
//

#include <unistd.h>
#include "dangerengine.h"
#include <Block.h>

int main(int argc, char **argv)
{
    slsContext *c = sls_context_new("window", 640, 640);

    slsMainLoop *mainloop = sls_mainloop_new(c);

    sls_msg(mainloop, run);
    sls_msg(mainloop, dtor);

    sls_log_info("hello!");
    sls_log_warn("warning %s", "does not compute");

    return 0;

}
