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

  slsMesh *earth_mesh;

  slsLightBatch lights;

  EarthModel earth;

  double date;

  double days_per_second;

  kmMat4 earth_transform;
  kmMat4 sun_transform;
  kmMat4 projection;
  slsTexture *earth_tex;
} EarthData;


static EarthData data = {
    .earth = (EarthModel) {
        .aphelion=152100000.0, // km
        .perihelion=147095000.0, // km
        .semi_major_axis=149598023, // km
        .inclination=7.155 * M_PI / 180, // in radians
        .period=365.256363 // days


    },


    .days_per_second = 10.0,
    .date = 0.0 // days
};

static slsContext *single_ctx = NULL;


void create_earth_mesh(slsMesh *self);

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


  glEnable(GL_CULL_FACE);
  sls_shader_init(&data.earth_shader, self->pool,
                  sls_create_program("resources/shaders/earth.vert",
                                     "resources/shaders/earth.frag",
                                     "resources/shaders/earth_uniforms.glsl"));


  data.earth_mesh = parametric_sphere_mesh(60);
  data.earth_mesh->gl_draw_mode = GL_TRIANGLES;


  data.earth_tex =
      sls_texture_new("resources/art/bm_default.jpg",
                      "resources/art/homer.jpg",
                      "resources/art/homer.jpg");

  sls_msg(data.earth_tex, set_program, data.earth_shader.program);
  sls_msg(data.earth_tex, bind);


  size_t max_lights = 8; // defined in shader
  sls_lightbatch_init(&data.lights, max_lights, NULL, 0);


  slsLight sun = {
      .ambient_product = {0.01, 0.01, 0.01},
      .diffuse_product = {0.9, 1.0, 0.8},
      .specular_product = {1.f, 1.f, 1.f},
      .light_position = {0.f, 0.f, 0.f}
  };

  sls_lightbatch_set_light(&data.lights, 0, &sun, true);


  glEnable(GL_PROGRAM_POINT_SIZE);

  glEnable(GL_BLEND_COLOR);

  sls_msg(data.earth_mesh, bind, &data.earth_shader);
}


void earth_ctx_teardown(slsContext *self)
{
  sls_shader_dtor(&data.earth_shader);
  sls_mesh_dtor(data.earth_mesh);

  sls_lightbatch_dtor(&data.lights);

  sls_context_class()->teardown(self);
}


void earth_ctx_update(slsContext *self, double dt)
{
  data.date += dt * data.days_per_second * 10;

  kmVec3 earth_pos = get_earth_position(data.date, &data.earth);
  //sls_log_info("earth at day %f: %f %f %f", data.date, earth_pos.x, earth_pos.y, earth_pos.z);

  kmVec3 sun_rel;
  kmVec3Scale(&sun_rel, &earth_pos, -1.f);

  kmMat4Translation(&data.sun_transform,
                    sun_rel.x,
                    sun_rel.y,
                    sun_rel.z);
  data.lights.light_modelviews[0] = data.sun_transform;
  GLuint program = data.earth_shader.program;

  GLuint ambient_products =
      (GLuint) glGetUniformLocation(program, "ambient_products");
  GLuint diffuse_products =
      (GLuint) glGetUniformLocation(program, "diffuse_products");
  GLuint specular_products =
      (GLuint) glGetUniformLocation(program, "specular_products");
  GLuint light_positions =
      (GLuint) glGetUniformLocation(program, "light_positions");
  GLuint light_modelviews =
      (GLuint) glGetUniformLocation(program, "light_modelviews");
  GLuint n_lights = (GLuint) glGetUniformLocation(program, "n_lights");

  glUniform3fv(ambient_products,
               data.lights.max_lights,
               (GLfloat *) data.lights.ambient_products);
  glUniform3fv(specular_products,
               data.lights.max_lights,
               (GLfloat *) data.lights.specular_products);
  glUniform3fv(diffuse_products,
               data.lights.max_lights,
               (GLfloat *) data.lights.diffuse_products);
  glUniform4fv(light_positions,
               data.lights.max_lights,
               (GLfloat *) data.lights.light_positions);

  glUniformMatrix4fv(light_modelviews,
                     data.lights.max_lights,
                     GL_FALSE,
                     (GLfloat *) data.lights.light_modelviews);

  glUniform1i(n_lights, (GLint) data.lights.n_lights);


}

void earth_ctx_display(slsContext *self, double dt)
{

  static float x_rot = NAN;
  if (isnan(x_rot)) {
    x_rot = 0.f;
  } else {
    x_rot += 0.5 * dt;
  }
  glUseProgram(data.earth_shader.program);
  kmMat4 ma, mb, mv;


  bool do_rotate = true;
  if (do_rotate ){
    kmQuaternion qa, qb, rot;
    kmVec3 axis = {0.f, 0.f, 1.f};
    kmQuaternionRotationAxisAngle(&qa, &axis, (float)data.earth.inclination);
    axis = (kmVec3){0.5f, -1.f, 0.f};
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
  double mean_anomaly = n * (time - tau);


  //TODO: calculate position from true anomaly via kepler's equation
  kmVec3 unit = {
      .x = (float) cos(mean_anomaly),
      .y = (float) sin(mean_anomaly),
      .z = 0.f};

  kmMat4 inclination;
  kmVec3Scale(&result, &unit, model->semi_major_axis);

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