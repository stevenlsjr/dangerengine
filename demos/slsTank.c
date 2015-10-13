/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/
#include "slsTank.h"

#include <state/slsSprite.h>

slsEntity *sls_create_tank(slsAppState *state,
                           char const *name,
                           kmVec2 position,
                           float direction,
                           bool player_controlled,
                           slsTexture *tex,
                           slsShader *shader)
{
  slsEntity *self = malloc(sizeof(slsEntity));

  sls_init_sprite(self, state, state->pool, name, tex, shader);

  self->component_mask |=
      SLS_COMPONENT_BEHAVIOR |
      SLS_COMPONENT_KINETIC;
  self->behavior.data = apr_pcalloc(self->pool, sizeof(slsTankData));

  slsTankData *data = self->behavior.data;
  self->behavior.update = sls_tankb_update;

  slsComponentMask barrel_mask = SLS_COMPONENT_NONE;

  sls_entity_init(&data->turret, self->pool, barrel_mask, "barrel0");
  sls_init_sprite(&data->barrel,
                  state, self->pool,
                  "turret0", tex, shader);

  // set transformation
  data->barrel.transform.pos = (kmVec2) {0, -0.5f};
  data->barrel.transform.scale = (kmVec2) {0.25, 0.5};
  data->barrel.transform.z_layer = -1;

  data->turret.transform.pos = (kmVec2) {0.0, 0.0};


  data->barrel.component_mask &= ~SLS_COMPONENT_KINETIC;
  data->turret.component_mask &= ~SLS_COMPONENT_KINETIC;

  // set kinematic
  self->kinematic = (slsKinematic2D) {
      .mass = 1.0,
      .velocity = {0.0, 0.0},
      .linear_drag = 0.05,
      .rotational_drag = 1.0,

      .rotational_inertia = 1.0,
      .rotational_speed = 0.0
  };

  sls_entity_addchild(self, &data->turret);
  sls_entity_addchild(&data->turret, &data->barrel);

  assert(data->turret.parent == self);
  assert(data->barrel.parent == &data->turret);


  data->acceleration = 6.0;
  data->max_speed = 20.0;
  data->rotational_speed = 70.0;

  return self;
}

void sls_tankb_update(slsBehavior *behavior, slsAppState *state, double dt)
{
  slsTankData *data = behavior->data;
  slsPlayerInput *in = &state->input;
  slsEntity *self = behavior->entity;
  slsIPoint control_axis = {0, 0};
  if (in->key_down) {
    control_axis.y -= 1;
  }
  if (in->key_up) {
    control_axis.y += 1;
  }
  if (in->key_left) {
    control_axis.x -= 1;
  }
  if (in->key_right) {
    control_axis.x += 1;
  }


  if (control_axis.y != 0) {
    double mag = control_axis.y * data->acceleration;

    // perpendicular to given rotation
    kmVec2 unit = {sinf(self->transform.rot), -cosf(self->transform.rot)};
    kmVec2 accel;
    kmVec2Scale(&accel, &unit, (float) mag);

    self->kinematic.velocity.x += accel.x;
    self->kinematic.velocity.y += accel.y;


  }


  if (control_axis.x != 0) {
    self->transform.rot -= ((float) control_axis.x * dt * data->rotational_speed);
  }


}
