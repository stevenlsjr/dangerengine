/**
 * @file ${FILE}
 * @brief  
 *
 *
 * NOTE: Forgive me for using an rendering
 * engine I have been working on for a while. Being a
 * solo project in C, it inevitably has some odd idioms
 * that I have used to simplify prototyping.
 */

#include "lab2-demo.h"

#include "render_demo.h"
#include "../src/dangerengine.h"

typedef struct labData {

  GLuint program;

  slsMesh *sphere;
  slsMesh *square_a;
  slsMesh *square_b;
  slsMesh *polygon;

  struct {
    GLint time_;
    GLint projection;
    GLint model_view;
    GLint normal_mat;
    GLint tex_sample;
  } uniforms;

} labData;


void lab1_setup_shaders(slsContext *self) __attribute__((nonnull(1)));

void lab1_setup_scene(slsContext *self) __attribute__((nonnull(1)));


void lab1_uniform_locations(labData *data)
{
  assert(data);
  data->uniforms.time_ = glGetUniformLocation(data->program, "time");
  data->uniforms.model_view = glGetUniformLocation(data->program, "model_view");
  data->uniforms.projection = glGetUniformLocation(data->program, "projection");
  data->uniforms.normal_mat = glGetUniformLocation(data->program, "normal_mat");
  data->uniforms.tex_sample = glGetUniformLocation(data->program, "diffuse_map");
}


void lab1_setup_shaders(slsContext *self)
{
  if (!self->data) { exit(EXIT_FAILURE); }
  char const *fs_path = "resources/shaders/lab.frag";
  char const *vs_path = "resources/shaders/lab.vert";

  labData *data = self->data;


  data->program = sls_create_program(vs_path, fs_path);
}

void lab1_context_setup(slsContext *self)
{
  if (!self) { exit(EXIT_FAILURE); }

  sls_context_class()->setup(self);

  self->data = calloc(sizeof(labData), 1);

  labData *data = self->data;


  lab1_setup_shaders(self);


  lab1_setup_scene(self);


  int x, y;
  glfwGetWindowSize(self->window, &x, &y);
  self->resize(self, x, y);

  glClearColor(0.1f, 0.24f, 0.3f, 1.0f);



  return;

}


void lab1_context_teardown(slsContext *self)
{
  labData *data = self->data;

  if (data) {

    glDeleteProgram(data->program);
    if (data->sphere) {sls_msg(data->sphere, dtor);}
    if (data->square_a) {sls_msg(data->square_a, dtor);}
    if (data->square_b) {sls_msg(data->square_b, dtor);}
    if (data->polygon) {sls_msg(data->polygon, dtor);}
    free(data);
  }
}

void lab1_setup_scene(slsContext *self)
{


  labData *data = self->data;

  kmVec4 color = {0.5, 0.5, 0.0, 1.0};
  data->sphere = sls_sphere_mesh(10, &color);


  slsVertex square_verts[] = {
      (slsVertex) {.position= {-1.0, -1.0, 0.0},
          .normal={0.0, 0.0, 1.0},
          .uv= {0.0, 0.0},
          .color={1.0, 1.0, 1.0, 1.0}},
      (slsVertex) {.position= {-1.0, 1.0, 0.0},
          .normal={0.0, 0.0, 1.0},
          .uv= {0.0, 1.0},
          .color={1.0, 1.0, 1.0, 1.0}},
      (slsVertex) {.position= {1.0, 1.0, 0.0},
          .normal={0.0, 0.0, 1.0},
          .uv= {1.0, 1.0},
          .color={1.0, 1.0, 1.0, 1.0}},
      (slsVertex) {.position= {1.0, -1.0, 0.0},
          .normal={0.0, 0.0, 1.0},
          .uv= {1.0, 0.0},
          .color={1.0, 1.0, 1.0, 1.0}}
  };

  uint32_t indices[] = {0, 1, 2, 3, 2, 0};

  data->square_b = sls_mesh_new(square_verts, 4,
                                indices, 6);


  // calls function pointer which binds vertex data to
  // appropriate vbo/ibo
  sls_msg(data->sphere, bind, data->program);


  return;
  error:
  getchar();
  exit(EXIT_FAILURE);
}



void lab1_setup_textures(slsContext *self)
{
  char const *img_path = "resources/free-textures/151.jpg";
  char const *norm_path = "resources/free-textures/151.jpg";

  labData *data = self->data;


}

void lab1_context_update(slsContext *self, double dt)
{
  labData *data = self->data;


}


void lab1_context_display(slsContext *self, double dt)
{

  labData *data = self->data;

  glUseProgram(data->program);



  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  sls_msg(data->square_b, pre_draw, data->program, dt);
  sls_msg(data->square_b, draw, dt);
  sls_msg(data->square_b, post_draw, data->program, dt);

  glfwSwapBuffers(self->window);
}


void lab1_context_resize(slsContext *self, int x, int y)
{

  labData *data = self->data;

  sls_log_info("%i %i", x, y);

  //glViewport(0, 0, x,  y);

  if (x != 0 && y!= 0) {
    float aspect = x / (float) y;

    kmMat4 projection;
    kmMat4OrthographicProjection(&projection,
                                 -aspect, aspect,
                                 -1.0f, 1.0f,
                                 -10.0f, 10.0f);
  }

}


int lab1_main(int *argc, char **argv)
{
  int w = 640;
  int h = 480;
  slsContext *c = sls_context_new("window", w, h);

  c->data = NULL;

  c->setup = lab1_context_setup;
  c->update = lab1_context_update;
  c->display = lab1_context_display;
  c->teardown = lab1_context_teardown;
  c->resize = lab1_context_resize;



  assert(c);

  c->resize(c, w, h);

  c->run(c);
  c->dtor(c);


  return 0;
}
