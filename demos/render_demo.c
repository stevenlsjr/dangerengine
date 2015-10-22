//
// Created by Steven on 7/2/15.
//

#include "render_demo.h"
#include "slsTank.h"
#include "tile_map.h"
#include <assert.h>
#include <renderer/slsshader.h>


#pragma mark - private declarations
void demo_setup_tilemap(slsContext *self);

void demo_handle_event(slsContext *self,
                       const SDL_Event *e) SLS_NONNULL(1);


void demo_handle_event(slsContext *self,
                       const SDL_Event *e)
{

  sls_context_class()->handle_event(self, e);
  demoData *data = self->data;

  data->mouse_motion = (slsIPoint) {0, 0};
  data->camera_move_input = (slsIPoint) {0, 0};

  switch (e->type) {
    case (SDL_MOUSEMOTION): {
      SDL_MouseMotionEvent me = e->motion;
      if (me.state & SDL_MOUSEBUTTONDOWN) { // mouse button is down
        data->mouse_motion.x += me.xrel;
        data->mouse_motion.y -= me.yrel;
      }

      break;
    }

    default:
      break;
  }
}

#pragma mark -demo setup
void demo_context_setup(slsContext *self)
{
#ifndef SLS_GNU_EXT
  if (!self) { exit(EXIT_FAILURE); }
#endif

  sls_context_class()->setup(self);

  self->data = calloc(sizeof(demoData), 1);

  sls_checkmem(self->data);

  demoData *data = self->data;
  {
    const size_t initial_size = 16;
    data->model_view = *sls_matrix_stack_init(&data->model_view,
                                              initial_size);
  }

  sls_checkmem(data->model_view.matrices);


  demo_setup_shaders(self);

  demo_setup_text(self);

  demo_setup_textures(self);

  demo_setup_scene(self);


  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);
  return;
  error:
  exit(EXIT_FAILURE);

}

void demo_setup_shaders(slsContext *self)
{
#ifndef SLS_GNU_EXT
  if (!self) {exit(EXIT_FAILURE);}
#endif
  if (!self->data) { exit(EXIT_FAILURE); }
  char const *fs_path = "resources/shaders/default.frag";
  char const *vs_path = "resources/shaders/default.vert";

  demoData *data = self->data;


  data->program = sls_create_program(vs_path, fs_path, NULL);
  data->shader =
      sls_shader_init(apr_pcalloc(self->state->pool,
                                  sizeof(slsShader)),
                      self->state->pool,
                      data->program);

  self->state->active_shader = data->shader;

}


void demo_setup_textures(slsContext *self)
{

  demoData *data = self->data;


  data->tank_tex = sls_texture_new("resources/art/tankBeige_outline.png",
                                   "resources/art/tankBeige_specular.png",
                                   "resources/art/tankBeige_normal.png");

  data->barrel_tex = sls_texture_new("resources/art/barrelBeige_outline.png",
                                     "resources/art/barrelBeige_specular.png",
                                     "resources/art/barrelBeige_normal.png");

  data->grass_tex = sls_texture_new("resources/art/sand.png",
                                    "resources/art/sand_spec.png",
                                    "resources/art/sand_normal.png");

  data->block_tex = sls_texture_new("resources/art/treeLarge.png",
                                    "resources/art/treeLarge_spec.png",
                                    "resources/art/treeLarge_normal.png");
}

void demo_setup_text(slsContext *self)
{

}

void demo_setup_tilemap(slsContext *self)
{
  demoData *data = self->data;

  data->grass = NULL;

  int map[]= {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
      1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
      1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
      1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
      1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };
  slsTilemapData tilemap = {
      .height = 12,
      .width = 12,
      .map = map,
      .tile_shader = data->shader,
      .tile_texture = data->grass_tex,
      .block_texture = data->block_tex
  };
  data->grass = sls_create_tilemap(self->state, "grass", (kmVec2) {0.0, 0.0}, 0.0, &tilemap);

  data->grass->transform.z_layer = 1;
  data->grass->transform.scale = (kmVec2){2.0, 2.0};

  data->grass->component_mask = data->grass->component_mask & ~SLS_COMPONENT_KINETIC;
  sls_entity_addchild(self->state->root, data->grass);

}

void demo_setup_scene(slsContext *self)
{

  demoData *data = self->data;

  glUseProgram(data->program);

  sls_camera_init(&data->camera);

  self->state->active_camera = sls_camera_init(&data->camera);


  // setup scene graph and tank

  self->state->root =
      sls_entity_new(self->pool, SLS_COMPONENT_NONE, "root");

  slsEntity *root = self->state->root;
  sls_checkmem(root);

  root->transform.scale = (kmVec2) {0.1, 0.1};

  data->tank = malloc(sizeof(slsSprite));

  data->tank  = sls_create_tank(self->state, "player_tank",
                                (kmVec2) {0.0, 0.0}, 0.0, true, data->tank_tex, data->barrel_tex,
                                data->shader);

  data->tank->transform.rot = M_PI;
  data->tank->transform.pos = (kmVec2){3.0, 3.0};
  demo_setup_tilemap(self);

  sls_entity_addchild(root, data->tank );

  sls_sprite_set_color(data->tank, (kmVec4){0.0, 1.0, 0.1}, true);


  // turn sprite graphics on/off
  //((slsTankData*)data->tank->behavior.data)->barrel.component_mask &= ~SLS_COMPONENT_MESH;
  //data->tank->component_mask &= ~SLS_COMPONENT_MESH;
  //data->grass->component_mask &= ~SLS_COMPONENT_MESH;

  return;

  error:
  assert(0);
  return;

}

#pragma mark -context callbacks

void demo_context_update(slsContext *self, double dt)
{

  sls_context_class()->update(self, dt);

  demoData *data = self->data;

  //sls_log_info("%f", dt);

  data->camera.transform.pos =
      sls_transform2d_local_to_world(&data->tank->transform,
                                     &self->state->work_stack, NULL);

  //data->camera.transform.rot;

}


void demo_update_uniforms(slsContext *self, double dt)
{
  demoData *data = self->data;
  glUniform1f(data->shader->uniforms.time, clock() / (float) CLOCKS_PER_SEC);
}


void demo_context_display(slsContext *self, double dt)
{
  sls_context_class()->display(self, dt);
}

void demo_context_teardown(slsContext *self)
{
  demoData *data = self->data;

  if (data) {

    glDeleteProgram(data->program);

    sls_msg(data->tank_tex, dtor);
    sls_msg(data->barrel_tex, dtor);

    if (data->tank) {
      sls_entity_delete(data->tank);
    }
    if (data->grass) {
      sls_msg(data->grass, dtor);
    }
  }

}

void demo_context_resize(slsContext *self, int x, int y)
{

  sls_context_class()->resize(self, x, y);

}


int render_demo_main(int *argc, char **argv)
{
  size_t w = 640;
  size_t h = 480;
  slsContext *c = sls_context_new("window", w, h);

  c->data = NULL;

  c->setup = demo_context_setup;
  c->update = demo_context_update;
  c->display = demo_context_display;
  c->teardown = demo_context_teardown;
  c->resize = demo_context_resize;
  c->handle_event = demo_handle_event;


  assert(c);


  sls_msg(c, run);
  sls_msg(c, dtor);


  return 0;
}

