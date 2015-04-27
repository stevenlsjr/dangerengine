//
// Created by Steven on 4/25/15.
//

#ifndef DANGERENGINE_SLSCONTEXT_H
#define DANGERENGINE_SLSCONTEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct slsContext slsContext;

struct slsContext {
    slsContext * (*init)(slsContext *self,
                         char const *caption,
                         size_t width, size_t height);
    void (*dtor)(slsContext *self);

    GLFWwindow *window;
};

slsContext const *sls_context_class();

slsContext *sls_context_new(char const *caption, size_t width, size_t height);

#endif //DANGERENGINE_SLSCONTEXT_H
