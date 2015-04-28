//
// Created by Steven on 4/27/15.
//

#ifndef DANGERENGINE_SLSLOOP_H
#define DANGERENGINE_SLSLOOP_H

#include "slscontext.h"
#include <time.h>


typedef struct slsMainLoop slsMainLoop;
typedef struct slsMainLoop_p slsMainLoop_p;


struct slsMainLoop {

    slsMainLoop *(*init)(slsMainLoop *self, slsContext *ctx);
    void (*dtor)(slsMainLoop *self);

    void (*run)(slsMainLoop *self);

    void (*handle_events)(slsMainLoop *self, double dt);

    void (*update)(slsMainLoop *self, double dt);
    void (*display)(slsMainLoop *self, double dt);

    clock_t interval;
    slsMainLoop_p *priv;
};

const slsMainLoop *sls_mainloop_class();
slsMainLoop *sls_mainloop_new(slsContext *ctx);



#endif //DANGERENGINE_SLSLOOP_H


