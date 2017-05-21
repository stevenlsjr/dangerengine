/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 *
 **/
#include "slsAppState.h"
#include <math/math-types.h>
#include <renderer/shaderutils.h>

slsAppState* sls_app_state_init(slsAppState* self, slsRendererGL* renderer)
{
  *self = (slsAppState){};
  self->input = sls_inputstate_new();
  sls_checkmem(self->input);

  slsMesh* mesh = sls_mesh_square(&self->mesh);
  sls_checkmem(mesh);

  GLuint program = sls_create_program("./resources/shaders/debug.vert",
                                      "./resources/shaders/debug.frag",
                                      "./resources/shaders/uniforms.glsl");
  sls_check(sls_shader_init(&self->mesh_shader, program),
            "active_shader failed");

  sls_mesh_setup_buffers(&self->mesh, &self->mesh_shader);
  self->renderer = renderer;

  return self;

error:
  if (self) {
    return sls_app_state_deinit(self);
  } else {
    return NULL;
  }
}

slsAppState* sls_app_state_deinit(slsAppState* self)
{
  sls_shader_dtor(&self->mesh_shader);
  sls_mesh_dtor(&self->mesh);
  if (self->input) {
    sls_inputstate_delete(self->input);
  }

  return self;
}

void sls_app_state_update(slsAppState* self, double dt)
{
  sls_inputstate_update(self->input, dt);
  slsRendererGL* r = self->renderer;
  r->active_mesh = &self->mesh;
  r->active_shader = &self->mesh_shader;
}
