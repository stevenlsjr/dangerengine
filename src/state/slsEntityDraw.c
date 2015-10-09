/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/8/15, Steven
 * 
 **/
#include "slsEntityDraw.h"
#include <state/slsEntity.h>
#include <renderer/slsshader.h>

void sls_entity_draw(slsEntity *self, double dt, slsAppState *state)
{
  if (self->mesh) {




    sls_msg(self->mesh, pre_draw, self->material->program, dt);
    sls_msg(self->mesh, draw, dt);
    sls_msg(self->mesh, post_draw, self->material->program, dt);

  }
}
