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

    sls_msg(self->mesh, pre_draw, self->shader->program, dt);
    sls_msg(self->mesh, draw, dt);
    sls_msg(self->mesh, post_draw, self->shader->program, dt);

  }
}

void sls_drawable_transform(slsEntity *self, slsAppState *state, double dt)
{

  slsMatrixStack *mv = &state->model_view;

  if (self == state->root) {
    sls_matrix_glidentity(mv);
  }

  sls_matrix_glscale(mv, (kmVec3) {self->transform.scale.x, self->transform.scale.y, 1.0});

  sls_matrix_gltranslate(mv, (kmVec3) {self->transform.pos.x,
                                       self->transform.pos.y,
                                       0.0});

  if (self->texture) {
    sls_msg(self->texture, bind);
  }

  if (self->shader) {
    sls_matrix_glbind(mv,
                      self->shader->program,
                      self->shader->uniforms.model_view,
                      self->shader->uniforms.normal_map);
  }

}
