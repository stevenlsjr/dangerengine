//
// Created by Steven on 4/25/15.
//

#include "slscontext.h"
#include "slsutils.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

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

    if (!glfwInit()) {
        return NULL;
    }

    self->window = glfwCreateWindow((int)width, (int)height, caption, NULL, NULL);
    if (self->window) {
        glfwMakeContextCurrent(self->window);
    } else {
        fprintf(stderr, "window construction failed\n");
        free(self);
        self = NULL;
    }

    return self;
}

void sls_context_dtor(slsContext *self)
{

    glfwTerminate();
    if (self) {
        free(self);
    }

}
