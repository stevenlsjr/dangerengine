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

  slsLightBatch lights;

  EarthModel earth;

  double date;

  double days_per_second;

  kmMat4 earth_transform;
  kmMat4 sun_transform;
  kmMat4 projection;
  slsTexture *earth_tex;


  slsTexture *earth_texb;
  bool do_rotate;
  bool do_orbit;
} EarthData;


static EarthData data = {
    .earth = (EarthModel) {
        .aphelion=152100000.0, // km
        .perihelion=147095000.0, // km
        .semi_major_axis=149598023, // km
        .inclination=7.155 * M_PI / 180, // in radians
        .period=365.256363, // days

        .radius=6371.0

    },


    .days_per_second = 20.0,
    .date = 0.0, // days

    .do_rotate = true,
    .do_orbit = false
};

static slsContext *single_ctx = NULL;


void create_earth_mesh(slsMesh *self);

void earth_bind_season(slsContext *pContext);

void earth_del_ctx(slsContext *ctx)
{
  assert(ctx == single_ctx);


  sls_context_class()->dtor(ctx);
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

  data.sun_mesh = parametric_sphere_mesh(10);
  data.sun_mesh->gl_draw_mode = GL_TRIANGLES;


  data.earth_tex =
      sls_texture_new("resources/art/june_bath.png",
                      "resources/art/dec_bath.png",
                      "resources/art/dec_bath.png");


  data.earth_texb = sls_texture_new("resources/art/june_bath.png",
                                    "resources/art/dec_bath.png",
                                    "resources/art/dec_bath.png");

  sls_msg(data.earth_tex, set_program, data.earth_shader.program);
  sls_msg(data.earth_tex, bind);


  size_t max_lights = 8; // defined in shader
  sls_lightbatch_init(&data.lights, max_lights, NULL, 0);


  slsLight sun = {
      .ambient_product = {0.01, 0.05, 0.1},
      .diffuse_product = {1.0, 1.0, 0.9},
      .specular_product = {1.f, 1.f, 1.f},
      .light_position = {0.f, 0.f, 0.f}
  };

  sls_lightbatch_set_light(&data.lights, 0, &sun, true);


  glEnable(GL_PROGRAM_POINT_SIZE);

  glClearColor(0.1, 0.2, 0.4, 1.0);

  //glEnable(GL_BLEND_COLOR);
  //glEnable(GL_CULL_FACE);


  sls_msg(data.earth_mesh, bind, &data.earth_shader);
}


void earth_ctx_teardown(slsContext *self)
{
  sls_shader_dtor(&data.earth_shader);

  sls_mesh_dtor(data.earth_mesh);
  sls_mesh_dtor(data.sun_mesh);

  sls_msg(data.earth_tex, dtor);
  sls_msg(data.earth_texb, dtor);

  sls_lightbatch_dtor(&data.lights);

  sls_context_class()->teardown(self);
}


void earth_ctx_update(slsContext *self, double dt)
{
  data.date += dt * data.days_per_second;

  kmVec3 earth_pos = get_earth_position(data.date, &data.earth);
  //sls_log_info("earth at day %f: %f %f %f", data.date, earth_pos.x, earth_pos.y, earth_pos.z);

  kmVec3 sun_rel;
  if (data.do_orbit) {
    double sun_theta = data.date / data.earth.period * 2 * M_PI;
    sun_rel = (kmVec3) {1.0, cos(sun_theta), sin(sun_theta)};
    kmVec3Scale(&sun_rel, &sun_rel, data.earth.semi_major_axis / data.earth.radius);
  } else {
    sun_rel = (kmVec3) {100.0, 100.0, -10000.0};
  }


  kmMat4Identity(&data.sun_transform);

  kmMat4Identity(&data.sun_transform);
  data.lights.light_modelviews[0] = data.sun_transform;
  data.lights.light_positions[0] = (kmVec4) {sun_rel.x, sun_rel.y, sun_rel.z, 1.0};
  GLuint program = data.earth_shader.program;

  size_t n_lights = data.lights.n_lights;

  slsShader *s = &data.earth_shader;

  //earth_bind_season(self);

  glUseProgram(s->program);

  glUniform1f(s->uniforms.time, (float) data.date);

  glUniform1i(s->uniforms.lights.n_lights, (GLint) data.lights.n_lights);

  glUniform3fv(s->uniforms.lights.ambient_products,
               n_lights,
               (GLfloat *) data.lights.ambient_products);
  glUniform3fv(s->uniforms.lights.specular_products,
               n_lights,
               (GLfloat *) data.lights.specular_products);

  glUniform3fv(s->uniforms.lights.diffuse_products,
               n_lights,
               (GLfloat *) data.lights.diffuse_products);
  glUniform4fv(s->uniforms.lights.light_positions,
               n_lights,
               (GLfloat *) data.lights.light_positions);

  glUniformMatrix4fv(s->uniforms.lights.light_modelview,
                     n_lights,
                     GL_FALSE,
                     (GLfloat *) data.lights.light_modelviews[0].mat);


}

void earth_bind_season(slsContext *pContext)
{
  int quarter = (int) (fmin(data.date / data.earth.period * 4.0, 4.0));

  GLuint sample_a = data.earth_shader.uniforms.diffuse_tex;
  GLuint sample_b = data.earth_shader.uniforms.specular_tex;

  slsShader *shader = &data.earth_shader;

  GLuint s1 = 0, s2 = 0;


  switch (quarter) {
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

  sls_log_info("q%i %u %u", quarter, s1, s2);


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, s1);

  glUniform1i(sample_a, 0);


  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, s2);

  glUniform1i(sample_b, 1);

}

void earth_ctx_display(slsContext *self, double dt)
{

  float x_rot = data.date * 2 * M_PI;

  glUseProgram(data.earth_shader.program);
  kmMat4 ma, mb, mv;


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


  kmMat4Translation(&ma, 0.f, 0.f, -10.f);


  kmMat4Multiply(&mv, &ma, &mb);

  kmMat4 normal;
  sls_mat4_normalmat(&normal, &mv);


  glUniformMatrix4fv(data.earth_shader.uniforms.model_view, 1, GL_FALSE, mv.mat);
  glUniformMatrix4fv(data.earth_shader.uniforms.normal_mat, 1, GL_FALSE, normal.mat);


  _sls_mesh_roughdraw(data.earth_mesh, data.earth_shader.program, dt);

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

  sls_context_class()->resize;

  const float fov = 60;
  float aspect = x / (float) y;
  kmMat4PerspectiveProjection(&data.projection, fov, aspect, -1.f, 1.f);

  //kmMat4OrthographicProjection(&data.projection, -5.f, 5.f, -5.f/aspect, 5.f/aspect, -100.f, 100.f);

  glUniformMatrix4fv(glGetUniformLocation(data.earth_shader.program, "projection"),
                     1, GL_FALSE, data.projection.mat);

}