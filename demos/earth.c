/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 * 
 **/
#include <renderer/slsshader.h>
#include "earth.h"
#include "sphere.h"


typedef struct EarthData {

  slsShader earth_shader;
  slsShader sun_shader;


  slsMesh *earth_mesh;
  slsMesh *sun_mesh;

  slsMesh *fb_target;
  slsShader fb_shader;

  slsLightBatch lights;

  EarthModel earth;

  int time_multiplyer;

  double date;
  double days_per_second;

  slsTrackball trackball;

  kmMat4 camera_view;

  kmMat4 earth_transform;
  kmMat4 sun_transform;
  kmMat4 projection;
  slsMaterial *earth_tex;


  slsMaterial *earth_texb;
  bool do_rotate;
  bool do_orbit;


} EarthData;


struct {
  GLuint fbo;
  GLuint texture;
} frame_buffer;

void earth_move_trackball(slsContext *self, slsIPoint const *start_point, slsIPoint const *second_point,
                          slsIPoint const *window_size);

void earth_mv_setup(slsContext *self);

static EarthData data = {
    .earth = (EarthModel) {
        .aphelion=152100000.0, // km
        .perihelion=147095000.0, // km
        .semi_major_axis=149598023, // km
        .inclination=7.155 * M_PI / 180, // in radians
        .period=365.256363, // days

        .radius=6371.0
    },

    .time_multiplyer = 1,


    .days_per_second = 0.5f,
    .date = 0.0, // days

    .do_rotate = true,
    .do_orbit = true
};

static slsContext *single_ctx = NULL;


void earth_del_ctx(slsContext *ctx)
{
  assert(ctx == single_ctx);


  free(sls_context_class()->dtor(ctx));
  single_ctx = NULL;
}

void earth_exit()
{
  earth_del_ctx(earth_shared_ctx());
}


slsContext *earth_shared_ctx()
{
  if (single_ctx) {
    return single_ctx;
  } else {
    slsContext *self = sls_context_new("earth", 640, 480);

    self->data = &data;

    self->dtor = NULL;

    self->setup = earth_ctx_setup;
    self->teardown = earth_ctx_teardown;
    self->update = earth_ctx_update;
    self->display = earth_ctx_display;

    self->handle_event = earth_handle_event;

    self->resize = earth_ctx_resize;
    single_ctx = self;

    atexit(earth_exit);
    return self;
  }
}


int earth_main(int *argcr, char **argv)
{
  slsContext *ctx = earth_shared_ctx();

  if (ctx) {
    sls_msg(ctx, run);
  }
  return 0;
}


//---------------------------------context callbacks---------------------------------------

void earth_ctx_setup(slsContext *self)
{
  sls_context_class()->setup(self);

  earth_mv_setup(self);


  sls_shader_init(&data.earth_shader, self->pool,
                  sls_create_program("resources/shaders/earth.vert",
                                     "resources/shaders/earth.frag",
                                     "resources/shaders/earth_uniforms.glsl"));

  //sls_shader_init(&data.sun_shader, self->pool,
  //                sls_create_program("resources/shaders/sun.vert",
  //                                   "resources/shaders/sun.frag",
  //                                   "resources/shaders/earth_uniforms.glsl"));

  data.sun_shader.owns_program = true;
  data.earth_shader.owns_program = true;

  data.earth_mesh = parametric_sphere_mesh(60);
  data.earth_mesh->gl_draw_mode = GL_TRIANGLES;

  data.earth_tex =
      sls_texture_new("resources/art/june_bath.png",
                      "resources/art/dec_bath.png",
                      "resources/art/clouds.png");


  data.earth_texb = sls_texture_new("resources/art/june_bath.png",
                                    "resources/art/dec_bath.png",
                                    "resources/art/clouds.png");

  if (glIsProgram(data.earth_shader.program)) {
    sls_msg(data.earth_tex, set_program, data.earth_shader.program);
    sls_msg(data.earth_tex, bind);
  }


  earth_setup_framebuffer(self);


  size_t max_lights = 8; // defined in shader
  sls_lightbatch_init(&data.lights, max_lights, NULL, 0);


  slsLight sun = {
      .ambient_product = {0.2, 0.1, 0.25},
      .diffuse_product = {1.0, 1.0, 0.9},
      .specular_product = {0.1f, 0.1f, 0.1f},
      .light_position = {0.f, 0.f, 0.f}
  };

  sls_lightbatch_set_light(&data.lights, 0, &sun, true);


  glEnable(GL_PROGRAM_POINT_SIZE);

  glClearColor(0.1, 0.2, 0.4, 1.0);

  //glEnable(GL_BLEND_COLOR);
  //glEnable(GL_CULL_FACE);


  sls_msg(data.earth_mesh, bind, &data.earth_shader);
}

void earth_mv_setup(slsContext *self)
{
  kmMat4Identity(&data.projection);
  kmMat4Identity(&data.sun_transform);
  kmMat4Identity(&data.earth_transform);

  sls_trackball_init(&data.trackball, 4.f, 2.f);
}

void earth_setup_framebuffer(slsContext *self)
{
  slsVertex verts[] = {
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}},
      (slsVertex) {.position={-1.0, -1.0, 0.0}, .normal={0.f, 0.f, 1.f}, .uv={0.f, 0.f}}
  };
  uint32_t indices[] = {0, 1, 2, 3, 2, 0};

  size_t n_verts = sizeof(verts) / sizeof(*verts);
  size_t n_indices = sizeof(indices) / sizeof(*indices);
  data.fb_target = sls_mesh_new(verts, n_verts, indices, n_indices);

  slsShader *res = sls_shader_init(&data.fb_shader,
                                   self->state->pool,
                                   sls_create_program("resources/shaders/postprocess.vert",
                                                      "resources/shaders/postprocess.frag",
                                                      "resources/shaders/earth_uniforms.glsl"));
  sls_check(res, "posprocess shader didn't initialzie");

  sls_mesh_bind(data.fb_target, &data.fb_shader);

  return;
  error:
  exit(EXIT_FAILURE);
}


void earth_ctx_teardown(slsContext *self)
{
  sls_shader_dtor(&data.earth_shader);
  sls_shader_dtor(&data.fb_shader);

  sls_mesh_dtor(data.earth_mesh);
  sls_mesh_dtor(data.sun_mesh);
  sls_mesh_dtor(data.fb_target);

  sls_msg(data.earth_tex, dtor);
  sls_msg(data.earth_texb, dtor);

  sls_lightbatch_dtor(&data.lights);

  sls_context_class()->teardown(self);
}


void earth_ctx_update(slsContext *self, double dt)
{

  sls_glerror_unwind();

  data.date += dt * data.days_per_second * data.time_multiplyer;

  kmVec3 earth_pos = get_earth_position(data.date, &data.earth);
  //sls_log_info("earth at day %f: %f %f %f", data.date, earth_pos.x, earth_pos.y, earth_pos.z);

  kmVec3 sun_rel;
  if (data.do_orbit) {
    double sun_theta = data.date / data.earth.period * 2 * M_PI;
    sun_rel = (kmVec3) {1.0, cosf(sun_theta), sinf(sun_theta)};
    kmVec3Scale(&sun_rel, &sun_rel, data.earth.semi_major_axis / data.earth.radius);
  } else {
    sun_rel = (kmVec3) {100.f, 100.f, -10000.f};
  }

  kmVec3 tmp = sun_rel;
  kmVec3MultiplyMat4(&sun_rel, &tmp, &data.trackball.rotation_mat);


  kmMat4Identity(&data.sun_transform);
  data.lights.light_modelviews[0] = data.sun_transform;
  data.lights.light_positions[0] = (kmVec4) {sun_rel.x, sun_rel.y, sun_rel.z, 1.0};
  GLuint program = data.earth_shader.program;

  size_t n_lights = data.lights.n_lights;

  slsShader *s = &data.earth_shader;
  slsLocationTable *tbl = &s->unif_table;

  earth_bind_season(self);

  glUseProgram(s->program);

  glUniform1f(sls_locationtable_get_val(tbl, "time"), (float) data.date);

  glUniform1i(sls_locationtable_get_val(tbl, "lights.n_lights"), (GLint) data.lights.n_lights);

  glUniform3fv(sls_locationtable_get_val(tbl, "lights.ambient_products"),
               n_lights,
               (GLfloat *) data.lights.ambient_products);
  glUniform3fv(sls_locationtable_get_val(tbl, "lights.specular_products"),
               n_lights,
               (GLfloat *) data.lights.specular_products);

  glUniform3fv(sls_locationtable_get_val(tbl, "lights.diffuse_products"),
               n_lights,
               (GLfloat *) data.lights.diffuse_products);
  glUniform4fv(sls_locationtable_get_val(tbl, "lights.light_positions"),
               n_lights,
               (GLfloat *) data.lights.light_positions);

  glUniformMatrix4fv(sls_locationtable_get_val(tbl, "lights.light_modelview"),
                     n_lights,
                     GL_FALSE,
                     (GLfloat *) data.lights.light_modelviews[0].mat);


}

void earth_bind_season(slsContext *pContext)
{

  float date_ratio = data.date / data.earth.period;

  int season = (int)
      fmin(date_ratio * 4.0, 4.0);


  slsShader *shader = &data.earth_shader;

  glUseProgram(shader->program);
  slsLocationTable *unifs = &shader->unif_table;

  GLuint sample_a = sls_locationtable_get_val(unifs, "diffuse_tex");
  GLuint sample_b = sls_locationtable_get_val(unifs, "specular_tex");

  GLint _season_blend =  glGetUniformLocation(shader->program, "season_blend");
  GLuint season_blend = (GLuint)_season_blend;

  GLuint s1 = 0, s2 = 0;


  switch (season) {
    case 0: {
      s1 = data.earth_texb->diffuse.gltex;
      s2 = data.earth_texb->specular.gltex;
    }
      break;
    case 1: {
      s1 = data.earth_texb->specular.gltex;
      s2 = data.earth_tex->diffuse.gltex;
    }
      break;
    case 2: {
      s1 = data.earth_tex->diffuse.gltex;
      s2 = data.earth_tex->specular.gltex;
    }
      break;
    case 3: {
      s1 = data.earth_tex->specular.gltex;
      s2 = data.earth_texb->diffuse.gltex;
    }
      break;

    default:
      break;
  }


  glUniform1f(season_blend, date_ratio);

#if 0

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, s1);

  glUniform1i(sample_a, 0);


  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, s2);

  glUniform1i(sample_b, 1);

#endif

}

void earth_ctx_display(slsContext *self, double dt)
{
  glClearColor(1.f, 0.f, 1.f, 1.f);


  float x_rot = -data.date * 2 * M_PI;

  glUseProgram(data.earth_shader.program);
  kmMat4 ma, mb, mv, scale;


  if (data.do_rotate) {
    kmQuaternion qa, qb, rot;
    kmVec3 axis = {0.f, 0.f, 1.f};
    kmQuaternionRotationAxisAngle(&qa, &axis, (float) data.earth.inclination);
    axis = (kmVec3) {0.f, 1.f, 0.f};
    kmQuaternionRotationAxisAngle(&qb, &axis, x_rot);

    kmQuaternionMultiply(&rot, &qa, &qb);
    kmMat4RotationQuaternion(&mb, &rot);

  } else {
    kmMat4Identity(&mb);
  }

  kmMat4 scene;

  kmMat4Translation(&ma, 0.f, 0.f, -5.f);
  kmMat4Scaling(&scale, 4.0, 4.0, 4.0);

  kmMat4Multiply(&scene, &ma, &data.trackball.rotation_mat);


  kmMat4Multiply(&mv, &scene, &mb);


  kmMat4 normal;
  sls_mat4_normalmat(&normal, &mv);

  slsLocationTable *unifs = &data.earth_shader.unif_table;

  glUniformMatrix4fv(sls_locationtable_get_val(unifs, "model_view"), 1, GL_FALSE, mv.mat);
  glUniformMatrix4fv(sls_locationtable_get_val(unifs, "normal_mat"), 1, GL_FALSE, normal.mat);


  //_sls_mesh_roughdraw(data.earth_mesh, data.earth_shader.program, dt);

  _sls_mesh_roughdraw(data.fb_target, data.fb_shader.program, dt);

}


kmVec3 get_earth_position(double date/*days*/, EarthModel *model)
{
  assert(model);
  kmVec3 result = {};


  double period = model->period;
  double n = 2 * M_PI * period; // mean angular motion

  double tau = 0.0; // time at pericenter

  double time = date;
  double mean_anomaly = (time - tau) / n;


  //TODO: calculate position from true anomaly via kepler's equation
  kmVec3 unit = {
      .x = (float) cos(mean_anomaly),
      .y = (float) sin(mean_anomaly),
      .z = 0.f};

  kmMat4 inclination;
  kmVec3Scale(&result, &unit, model->semi_major_axis);
//
  kmMat4RotationX(&inclination, model->inclination);

  unit = result;

  kmVec3MultiplyMat4(&result, &unit, &inclination);


  return result;
}


void earth_ctx_resize(slsContext *self, int x, int y)
{
  glUseProgram(data.earth_shader.program);

  sls_context_class()->resize(self, x, y);

  const float fov = 70.f;
  float aspect = x / (float) y;
  kmMat4PerspectiveProjection(&data.projection, fov, aspect, -1.f, 1.f);

  //kmMat4OrthographicProjection(&data.projection, -5.f, 5.f, -5.f/aspect, 5.f/aspect, -100.f, 100.f);

  glUniformMatrix4fv(glGetUniformLocation(data.earth_shader.program, "projection"),
                     1, GL_FALSE, data.projection.mat);

}

void earth_handle_event(slsContext *self, SDL_Event const *e)
{
  static void (*super)(slsContext *self, SDL_Event const *e) = NULL;
  if (!super) { super = sls_context_class()->handle_event; }

  static slsIPoint last_mouse = {-1, -1};


  super(self, e);

  switch (e->type) {
    case SDL_MOUSEMOTION: {

      SDL_MouseMotionEvent const *me = &e->motion;
      slsIPoint mouse = {me->x, me->y};

      if (me->state & SDL_BUTTON(SDL_BUTTON_LEFT)) {

        if (last_mouse.x == -1 && last_mouse.y == -1) {
          last_mouse = mouse;
        }
        bool no_motion = sls_ipoint_eq(&mouse, &last_mouse);

        if (!no_motion) {
          slsIPoint win_size = {};
          SDL_GetWindowSize(self->window, &win_size.x, &win_size.y);
          earth_move_trackball(self, &mouse, &last_mouse, &win_size);

        }
      }

      last_mouse = mouse;
    }
      break;

    case SDL_KEYDOWN: {
      SDL_KeyboardEvent const *ke = &e->key;
      switch (ke->keysym.scancode) {
        case SDL_SCANCODE_R: {
          kmMat4Identity(&data.trackball.rotation_mat);
          kmQuaternionIdentity(&data.trackball.rotation);
          data.time_multiplyer = 1;
          data.date = 0.0;
        }
          break;

        case SDL_SCANCODE_COMMA: {
          data.time_multiplyer--;
          sls_log_info("%i", data.time_multiplyer);

        }
          break;
        case SDL_SCANCODE_PERIOD: {
          data.time_multiplyer++;
          sls_log_info("%i", data.time_multiplyer);
        }
          break;

        case SDL_SCANCODE_SPACE: {
          sls_log_info("t %c%f days.", data.date > 0.0 ? '+' : '-', fabs(data.date));
        }
          break;


        default:
          break;
      }

    }
      break;

    default:
      break;
  }
}

void earth_move_trackball(slsContext *self,
                          slsIPoint const *start_point,
                          slsIPoint const *second_point,
                          slsIPoint const *window_size)
{

  float
      w = window_size->x,
      h = window_size->y;


  kmVec2 delta = {
      .x = (start_point->x - second_point->x) / w,
      .y = (second_point->y - start_point->y) / h
  };


  slsTrackball *ball = &data.trackball;

  kmVec2 param_0 = {(2.f * start_point->x - w) / w, (h - 2.f * start_point->y) / h};
  kmVec2 param_1 = {(2.f * second_point->x - w) / w, (h - 2.f * second_point->y) / h};


  sls_trackball_set(ball, param_0, param_1);
}
