/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/8/15, Steven
 * 
 **/
#include "slsEntityDraw.h"
#include <apr-1/apr_strmatch.h>
#include <apr-1/apr_strings.h>
#include <state/slsEntity.h>
#include <renderer/slsshader.h>
#include <slscontext.h>


void sls_entity_draw(slsEntity *self, double dt, slsAppState *state)
{


  apr_strmatch_pattern const *re = apr_strmatch_precompile(state->context->tmp_pool,
                                                           "tile.*",
                                                           0);
  if (apr_strmatch(re, self->name, 10000)) {
    sls_log_info("%s", self->name);
  };
  if (self->mesh && self->shader) {

    sls_msg(self->mesh, pre_draw, self->shader->program, dt);
    sls_msg(self->mesh, draw, dt);
    sls_msg(self->mesh, post_draw, self->shader->program, dt);

  }
}

void sls_drawable_transform(slsEntity *self, slsAppState *state, double dt)
{


  if (self->texture) {
    sls_msg(self->texture, bind);
  }

  if (self->shader) {
    sls_matrix_glbind(&state->model_view,
                      self->shader->program,
                      self->shader->uniforms.model_view,
                      self->shader->uniforms.normal_map);
  }

}
