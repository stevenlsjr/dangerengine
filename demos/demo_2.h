/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#ifndef DANGERENGINE_DEMO_H
#define DANGERENGINE_DEMO_H

#include <dangerengine.h>

typedef struct DemoModel DemoModel;

struct DemoModel {
};

slsContext *demo_shared_ctx();

void demo_del_ctx(slsContext *ctx) SLS_NONNULL(1);

void demo_ctx_update(slsContext *self, double dt) SLS_NONNULL(1);

void demo_ctx_display(slsContext *self, double dt) SLS_NONNULL(1);

void demo_ctx_setup(slsContext *self) SLS_NONNULL(1);

void demo_ctx_teardown(slsContext *self) SLS_NONNULL(1);

kmVec3 get_demo_position(double date, DemoModel *model) SLS_NONNULL(2);

void demo_ctx_resize(slsContext *self, int x, int y);

int demo_main(int *argcr, char **argv);

void demo_bind_season(slsContext *pContext);

void demo_setup_framebuffer(slsContext *self);

void demo_handle_event(slsContext *self, SDL_Event const *e) SLS_NONNULL(1);

#endif // DANGERENGINE_DEMO_H
