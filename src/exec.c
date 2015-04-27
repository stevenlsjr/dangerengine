//
// Created by Steven on 4/25/15.
//

#include <unistd.h>
#include "dangerengine.h"

int main(int argc, char **argv)
{
    slsContext *c = sls_context_new("window", 640, 640);

    sleep(2);
    if (c) {
        sls_msg(c, dtor);
    }
    return 0;
}
