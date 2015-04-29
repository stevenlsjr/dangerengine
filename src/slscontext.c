//
// Created by Steven on 4/25/15.
//

#include "slscontext.h"
#include "slsutils.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "sls-handlers.h"

slsContext * sls_context_init(slsContext *self,
                              char const *caption,
                              size_t width,
                              size_t height);



void sls_context_dtor(slsContext *self);

static const slsContext sls_context_proto = {
        .init = sls_context_init,
        .dtor = sls_context_dtor,
        .window = NULL
};


slsContext const *sls_context_class() {

    return &sls_context_proto;
}

slsContext *sls_context_new(char const *caption, size_t width, size_t height)
{
    slsContext *self = sls_objalloc(sls_context_class(), sizeof(slsContext));


    return self->init(self, caption, width, height);
}


slsContext *sls_context_init(slsContext *self,
                             char const *caption,
                             size_t width,
                             size_t height)
{
    if (!self) { return NULL; }
    *self = sls_context_proto;

    if (!sls_is_active()) {
        bool res = sls_init();
    }

    self->window = glfwCreateWindow((int)width, (int)height, caption, NULL, NULL);
    sls_check(self->window, "window creation failed");

    sls_bind_context(self);

    return self;
error:
    if (self && self->dtor) {
        sls_msg(self, dtor);
    }
    else if (self) {
        free(self);
    }
    return NULL;
}

void sls_context_dtor(slsContext *self)
{

    if (self) {
        free(self);
    }

}
