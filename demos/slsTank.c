/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/12/15, Steven
 * 
 **/
#include "slsTank.h"
#include "tile_map.h"

#include <state/slsSprite.h>

slsEntity *sls_tank_dtor(slsEntity *self);


void sls_tank_checkcollision(slsEntity *self,
                             slsTankData *data,
                             slsAppState *state,
                             double dt) SLS_NONNULL(1, 2, 3);

slsEntity *sls_create_tank(slsAppState *state,
                           char const *name,
                           kmVec2 position,
                           float direction,
                           bool player_controlled,
                           slsMaterial *tex,
                           slsMaterial *barrel_tex,
                           slsShader *shader)
{
  slsEntity *self = malloc(sizeof(slsEntity));

  sls_init_sprite(self, state->pool, name, tex, shader);

  self->component_mask |=
      SLS_COMPONENT_BEHAVIOR |
      SLS_COMPONENT_KINETIC;
  self->behavior.data = apr_pcalloc(self->pool, sizeof(slsTankData));

  slsTankData *data = self->behavior.data;
  self->behavior.update = sls_tankb_update;

  slsComponentMask barrel_mask = SLS_COMPONENT_NONE;

  sls_entity_init(&data->turret, self->pool, barrel_mask, "turret0");
  sls_init_sprite(&data->barrel, self->pool, "barrel0", barrel_tex, shader);

  // set transformation
  data->barrel.transform.pos = (kmVec2) {0, -0.5f};
  data->barrel.transform.scale = (kmVec2) {0.25, 0.5};
  data->barrel.transform.z_layer = -1;

  data->turret.transform.pos = (kmVec2) {0.0, 0.0};


  data->barrel.component_mask = data->barrel.component_mask & ~SLS_COMPONENT_KINETIC;
  data->turret.component_mask = data->turret.component_mask & ~SLS_COMPONENT_KINETIC;
  data->barrel.component_mask = data->barrel.component_mask & ~SLS_COMPONENT_BEHAVIOR;
  data->turret.component_mask = data->turret.component_mask & ~SLS_COMPONENT_BEHAVIOR;


  slsComponentMask m = data->turret.component_mask;
  assert((m & SLS_COMPONENT_KINETIC) !=
         SLS_COMPONENT_KINETIC);
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
  assert(data->barrel.parent->parent != &data->turret);

  data->tilemap = NULL;


  data->acceleration = 1.0;
  data->max_speed = 20.0;
  data->rotational_speed = 5.0;

  sls_matrix_stack_init(&data->work_stack, 8);

  self->dtor = sls_tank_dtor;

  return self;
}

void sls_tankb_update(slsEntity *self, slsAppState *state, double dt)
{

  slsTankData *data = self->behavior.data;
  slsPlayerInput *in = &state->input;
  slsIPoint control_axis = {0, 0};
  slsIPoint aim_axis = {0, 0};

  if (!data->tilemap) {
    data->tilemap = sls_entity_findnamed(self, "grass");
    if (!data->tilemap) { return; }
  }

  control_axis.y += (in->key_up) ? 1 : 0;
  control_axis.y += (in->key_down) ? -1 : 0;

  control_axis.x += (in->key_right) ? 1 : 0;
  control_axis.x += (in->key_left) ? -1 : 0;

  aim_axis.y += (in->aim_up) ? 1 : 0;
  aim_axis.y += (in->aim_down) ? -1 : 0;

  aim_axis.x += (in->aim_right) ? 1 : 0;
  aim_axis.x += (in->aim_left) ? -1 : 0;


  if (control_axis.y != 0) {
    double mag = control_axis.y * data->acceleration;

    // perpendicular to given rotation
    kmVec2 unit = {sinf(self->transform.rot), -cosf(self->transform.rot)};
    kmVec2 accel;
    kmVec2Scale(&accel, &unit, (float) mag);

    kmVec2 velocity = {self->kinematic.velocity.x + accel.x, self->kinematic.velocity.y + accel.y};

    if (kmVec2Length(&velocity) <= data->max_speed) {
      self->kinematic.velocity = velocity;
    } else {
      kmVec2Normalize(&velocity, &velocity);
      kmVec2Scale(&self->kinematic.velocity, &velocity, (float) data->max_speed);
    }
  }


  if (control_axis.x != 0) {
    self->transform.rot -= ((float) control_axis.x * data->rotational_speed) * dt;
  }

  // braking
  if (in->key_brake) {
    self->kinematic.linear_drag = 0.6;
  } else {
    self->kinematic.linear_drag = 0.01;
  }

  // turret aim
  if (aim_axis.x != 0 || aim_axis.y != 0) {
    float world_rot = atan2f((float) aim_axis.x, -(float) aim_axis.y);
    data->turret.transform.rot = sls_transform2d_world_to_local_angle(&self->transform, world_rot);

  } else {
    data->turret.transform.rot = 0;
  }


  kmVec2 world_pos = sls_transform2d_local_to_world(&self->transform, NULL);

  sls_tank_turret_update(&data->turret, state, dt * 10);

  sls_tank_checkcollision(self, data, state, dt);

}


slsEntity *sls_tank_dtor(slsEntity *self)
{

  slsTankData *data = self->behavior.data;
  if (data) {
    sls_matrix_stack_dtor(&data->work_stack);
  }

  sls_entity_dtor(self);

  return self;
}


void sls_tank_turret_update(slsEntity *self, slsAppState *state, double dt)
{
  kmVec4 pos = {self->transform.pos.x, self->transform.pos.y, 0.0, 1.0};
  kmVec4 world_pos;
  kmVec4MultiplyMat4(&world_pos, &pos, &self->transform.model_view);

  slsIPoint mouse = state->input.mouse_pos;

  kmVec2 vmouse = sls_ipoint_to_vec2(&mouse);
  kmVec4 v4mouse = {vmouse.x, vmouse.y, 0.0f, 1.0f};
  kmVec4 vmouse_world;

  kmMat4 inverse_proj;
  kmMat4Inverse(&inverse_proj, &state->projection);
  kmVec4MultiplyMat4(&vmouse_world, &v4mouse, &state->projection);

}


void sls_tank_checkcollision(slsEntity *self,
                             slsTankData *data, slsAppState *state, double dt)
{
  if (!data->tilemap) { return; }
  slsTilemapData *tm_data = data->tilemap->behavior.data;
  assert(tm_data && tm_data->child_sprites);

  slsTransform2D *dest = &tm_data->child_sprites[0].transform;

  kmVec2 world_pos = sls_transform2d_local_to_world(&self->transform, NULL);

  kmVec2 tile_local = sls_transform2d_world_to_local(dest, &world_pos);

  kmVec2 tile_coords;

  tile_coords.x = 0.0;
  tile_coords.y = 0.0;

  //sls_log_info("%f %f", floor(tile_local.x  + 0.5), floor(tile_local.y + 0.5));


}
