/**
 * @file slsmesh.c
 * @brief
 *
 * Copyright (c) 6/23/15, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea.
**/
#include "slsmesh.h"
#include "../slsutils.h"
#include "shaderutils.h"
#include "string.h"
#include "slsshader.h"

#include <assert.h>
#include <math/math-types.h>

slsMesh *sls_mesh_init(slsMesh *self,
                       slsVertex const *vertices,
                       size_t vert_count,
                       unsigned const *indices,
                       size_t idx_count);


void sls_mesh_dtor(slsMesh *self);

void sls_mesh_bind(slsMesh *self, slsShader *shader);

void _sls_mesh_binddata(slsMesh *self, GLuint program);

void _sls_mesh_bindattrs(slsMesh *self, GLuint program);


void sls_mesh_predraw(slsMesh *self, GLuint program, double dt);

void sls_mesh_draw(slsMesh *self, double dt);

void sls_mesh_postdraw(slsMesh *self, GLuint program, double dt);


struct slsMesh_p {
  int placeholder;
};

static const slsMesh sls_mesh_proto = {
    .init = sls_mesh_init,
    .dtor = sls_mesh_dtor,
    .bind = sls_mesh_bind,

    .pre_draw=sls_mesh_predraw,

    .draw=sls_mesh_draw,
    .post_draw= sls_mesh_postdraw,
    .vbo = 0,
    .ibo = 0,
    .vao = 0,
    .is_drawing = SLS_FALSE
};

/*================================
 * IMPLEMENTATIONS
 *================================*/

slsVertex sls_vertex_make(kmVec3 position,
                          kmVec3 normal,
                          kmVec2 uv,
                          kmVec4 color)
{
  slsVertex v;
  v.position[0] = position.x;
  v.position[1] = position.y;
  v.position[2] = position.z;

  v.normal[0] = normal.x;
  v.normal[1] = normal.y;
  v.normal[2] = normal.z;

  v.uv[0] = uv.x;
  v.uv[1] = uv.y;

  v.color[0] = color.x;
  v.color[1] = color.y;
  v.color[2] = color.z;
  v.color[3] = color.w;

  return v;
}

slsMesh const *sls_mesh_class()
{
  slsMesh const *proto = &sls_mesh_proto;
  return proto;
}

slsMesh *sls_mesh_new(slsVertex const *vertices,
                      size_t vert_count,
                      unsigned const *indices,
                      size_t idx_count)
{
  slsMesh *obj = sls_objalloc(sls_mesh_class(), sizeof(slsMesh));
  return obj->init(obj, vertices, vert_count, indices, idx_count);
}

slsMesh *sls_mesh_init(slsMesh *self,
                       slsVertex const *vertices,
                       size_t vert_count,
                       unsigned const *indices,
                       size_t idx_count)
{
  if (!vertices || !indices || !self) { return NULL; }
  self->indices = sls_array_new(indices, sizeof(unsigned), idx_count);
  self->vertices = sls_array_new(vertices, sizeof(slsVertex), vert_count);


  self->priv = calloc(sizeof(slsMesh_p), 1);
  sls_checkmem(self->priv);


  glGenBuffers(1, &self->vbo);
  glGenBuffers(1, &self->ibo);

  glGenVertexArrays(1, &self->vao);

  return self;
  error:
  if (self && self->dtor) { sls_msg(self, dtor); }
  return NULL;
}

void sls_mesh_dtor(slsMesh *self)
{
  if (!self) { return; }

  if (self->priv) { free(self->priv); }

  sls_msg(self->indices, dtor);
  sls_msg(self->vertices, dtor);

  GLuint buffers[] = {self->vbo, self->ibo};


  glDeleteBuffers(sizeof(buffers) / sizeof(GLuint), buffers);


  glDeleteVertexArrays(1, &(self->vao));
  free(self);
}


slsVertex *sls_mesh_get_verts(slsMesh *self, size_t *len_out)
{
  if (len_out) {
    *len_out = sls_array_length(self->vertices);
  }
  return (slsVertex *) sls_array_get(self->vertices, 0);
}

uint32_t *sls_mesh_get_indices(slsMesh *self, size_t *len_out)
{
  if (len_out) {
    *len_out = sls_array_length(self->indices);
  }
  return (uint32_t *) sls_array_get(self->indices, 0);
}


void sls_mesh_bind(slsMesh *self, slsShader *shader)
{
  if (!self) { return; }
  // bind gl objects
  glUseProgram(shader->program);

#ifndef SLS_GLES
  glBindVertexArray(self->vao);
#endif

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  _sls_mesh_binddata(self, shader->program);

  _sls_mesh_bindattrs(self, shader->program);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef SLS_GLES
  glBindVertexArray(0);
#endif


}


void _sls_mesh_binddata(slsMesh *self, GLuint program)
{
  if (!self) { return; }
  // bind gl objects
  glUseProgram(program);
#ifndef SLS_GLES
  glBindVertexArray(self->vao);
#endif

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  const size_t vbo_size = sls_array_length(self->vertices) * sls_array_element_size(self->vertices);
  const size_t ibo_size = sls_array_length(self->indices) * sls_array_element_size(self->indices);


  slsVertex const *verts = sls_array_get(self->vertices, 0);
  unsigned int const *idxs = sls_array_get(self->indices, 0);

  // push index buffer data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, idxs, GL_STATIC_DRAW);

  // push vertex buffer data
  glBufferData(GL_ARRAY_BUFFER, vbo_size, verts, GL_STATIC_DRAW);
}

void _sls_mesh_bindattrs(slsMesh *self, GLuint program)
{

  if (!self) { return; }

  glUseProgram(program);
#ifndef SLS_GLES
  glBindVertexArray(self->vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  glVertexAttribPointer(SLS_ATTRIB_POSITION,
                        3,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *) offsetof(slsVertex, position));
  glEnableVertexAttribArray(SLS_ATTRIB_POSITION);

  glVertexAttribPointer(SLS_ATTRIB_NORMAL,
                        3,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *) offsetof(slsVertex, normal));
  glEnableVertexAttribArray(SLS_ATTRIB_NORMAL);

  glVertexAttribPointer(SLS_ATTRIB_UV,
                        2,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *) offsetof(slsVertex, uv));
  glEnableVertexAttribArray(SLS_ATTRIB_UV);


  glVertexAttribPointer(SLS_ATTRIB_COLOR,
                        4,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *) offsetof(slsVertex, color));
  glEnableVertexAttribArray(SLS_ATTRIB_COLOR);

}


void _sls_mesh_roughdraw(slsMesh *self, GLuint program, double dt)
{

  self->pre_draw(self, program, dt);
  self->draw(self, dt);
  self->post_draw(self, program, dt);

}


void sls_mesh_predraw(slsMesh *self, GLuint program, double dt)
{
  glUseProgram(program);


  self->is_drawing = SLS_TRUE;
  // setup vert position pointer

  glBindVertexArray(self->vao);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

}

void sls_mesh_draw(slsMesh *self, double dt)
{

  size_t elements = sls_array_length(self->indices);
  glDrawElements(GL_TRIANGLES, (int) elements, GL_UNSIGNED_INT, NULL);

}

void sls_mesh_postdraw(slsMesh *self, GLuint program, double dt)
{
  self->is_drawing = SLS_FALSE;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//---------------------------------mesh generations---------------------------------------




slsVertex *sls_sphere_vertices(size_t n_vertices,
                               kmVec4 const *color)
{
  slsVertex *sphere = NULL;

  size_t sphere_size = n_vertices + 1;
  sphere = calloc(sphere_size, sizeof(slsVertex));

  for (int i = 0; i < n_vertices; ++i) {
    double theta = (M_PI * 2.0 * (double) i) / (double) n_vertices;

    float pos[3] = {(float) cos(theta), (float) sin(theta), 1.0f};

    slsVertex v = {.normal={0.0, 0.0, 1.0}, .uv={0.0, 0.0}};
    memcpy(v.position, pos, sizeof(float[3]));
    v.color[0] = color->x;
    v.color[1] = color->y;
    v.color[2] = color->z;
    v.color[3] = color->w;


  }

  return sphere;
}

slsMesh *sls_sphere_mesh(size_t n_vertices,
                         kmVec4 const *color)
{
  slsMesh *m = NULL;

  size_t n_triangles = n_vertices - 2;
  size_t n_elements = n_triangles * 3;
  uint32_t *elements = calloc(n_elements + 1, sizeof(uint32_t));
  slsVertex *verts = sls_sphere_vertices(n_vertices, color);


  // naive fan sphere triangulation
  int starting_pos = 1;
  for (int i = starting_pos; i < n_triangles; ++i) {

    assert(i + 2 < n_vertices);
    uint32_t triangle[3] = {0, (uint32_t) i, (uint32_t) i + 1};

    memcpy(elements + i, triangle, sizeof(uint32_t[3]));
  }

  m = sls_mesh_new(verts, n_vertices, elements, n_elements);

  free(elements);
  free(verts);

  return m;
}

slsMesh *sls_mesh_create_shape(char const *name)
{

  if (!name) { return NULL; }

  slsMesh *mesh = NULL;

  size_t max_count = 50;

  char const *cube = "cube";
  char const *sphere = "sphere";
  char const *square = "square";

  if (strncmp(name, cube, max_count) == 0) {
    sls_log_err("cube mesh not implemented");

  } else if (strncmp(name, sphere, max_count) == 0) {
    sls_log_err("sphere mesh not implemented");
  } else if (strncmp(name, square, max_count) == 0) {
    slsVertex verts[] = {
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


    unsigned int idxs[] = {0, 1, 2, 3, 2, 0};

    size_t n_verts = sizeof(verts) / sizeof(slsVertex);
    size_t n_idx = sizeof(idxs) / sizeof(unsigned int);

    mesh = sls_mesh_new(verts, n_verts, idxs, n_idx);

  } else {
    sls_log_err("invalid name given");
  }

  return mesh;

}


slsMesh *sls_mesh_square()
{
  slsVertex verts[] = {
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

  unsigned int idxs[] = {0, 1, 2, 3, 2, 0};

  size_t n_verts = sizeof(verts) / sizeof(slsVertex);
  size_t n_idx = sizeof(idxs) / sizeof(unsigned int);

  return sls_mesh_new(verts, n_verts, idxs, n_idx);
}

slsMesh *sls_tile_mesh(size_t width, size_t height)
{
  size_t quads = width * height;
  size_t n_verts = quads * 4;
  size_t n_indices = quads * 6;

  float unit_len = 1.0;

  slsArray *verts = sls_array_new((slsVertex[0]) {}, sizeof(slsVertex), 0);
  slsArray *idx = sls_array_new((uint32_t[0]) {}, sizeof(uint32_t), 0);

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      slsVertex quad[] = {
          (slsVertex) {.position={0.0, 0.0, 0.0}, .uv={0.0, 0.0}},
          (slsVertex) {.position={0.0, 1.0, 0.0}, .uv={0.0, 1.0}},
          (slsVertex) {.position={1.0, 1.0, 0.0}, .uv={1.0, 1.0}},
          (slsVertex) {.position={1.0, 0.0, 0.0}, .uv={1.0, 0.0}},
      };

      uint32_t quad_elems[] = {
          0, 1, 2,
          3, 2, 0
      };
      for (int x = 0; x < sizeof(quad) / sizeof(slsVertex); ++x) {
        slsVertex *v = quad + x;
        memcpy(v->normal, ((float[3]) {0.0, 0.0, 1.0}), sizeof(float[3]));
        memcpy(v->color, ((float[4]) {1.0, 1.0, 1.0, 1.0}), sizeof(float[4]));

        v->position[0] *= (float) i * unit_len;
        v->position[1] *= (float) j * unit_len;
        sls_array_append(verts, v);
      }
      size_t current_idx_count = sls_array_length(idx);

      for (int x = 0; i < sizeof(quad_elems) / sizeof(slsVertex); ++x) {
        uint32_t *e = quad_elems + x;
        *e += current_idx_count;
        sls_array_append(idx, e);

      }
    }

    free(verts->dtor(verts));
    free(idx->dtor(idx));
  }

  return NULL;
}

void sls_mesh_get_positions(slsMesh *self, kmVec3 *buffer_out, size_t buffer_len)
{
  size_t len = 0;
  slsVertex const *verts = sls_mesh_get_verts(self, &len);

  len = (len < buffer_len)? len: buffer_len;

  for (size_t i = 0; i < len; ++i) {
    slsVertex const *iter = verts + i;
    buffer_out[i] = sls_array_to_vec3(iter->position);
  }

}

void sls_mesh_get_normals(slsMesh *self, kmVec3 *buffer_out, size_t buffer_len)
{
  size_t len = 0;
  slsVertex const *verts = sls_mesh_get_verts(self, &len);

  len = (len < buffer_len)? len: buffer_len;

  for (size_t i = 0; i < len; ++i) {
    slsVertex const *iter = verts + i;
    buffer_out[i] = sls_array_to_vec3(iter->normal);
  }
}

void sls_mesh_get_uvs(slsMesh *self, kmVec2 *buffer_out, size_t buffer_len)
{
  size_t len = 0;
  slsVertex const *verts = sls_mesh_get_verts(self, &len);

  len = (len < buffer_len)? len: buffer_len;

  for (size_t i = 0; i < len; ++i) {
    slsVertex const *iter = verts + i;
    buffer_out[i] = sls_array_to_vec2(iter->uv);
  }
}

void sls_mesh_get_colors(slsMesh *self, kmVec4 *buffer_out, size_t buffer_len)
{
  size_t len = 0;
  slsVertex const *verts = sls_mesh_get_verts(self, &len);

  len = (len < buffer_len)? len: buffer_len;

  for (size_t i = 0; i < len; ++i) {
    slsVertex const *iter = verts + i;
    buffer_out[i] = sls_array_to_vec4(iter->color);
  }
}

void sls_mesh_set_positions(slsMesh *self, kmVec3 *buffer_in, size_t n_items)
{

}

void sls_mesh_set_normals(slsMesh *self, kmVec3 *buffer_in, size_t n_items)
{

}

void sls_mesh_set_uvs(slsMesh *self, kmVec2 *buffer_in, size_t n_items)
{

}

void sls_mesh_set_colors(slsMesh *self, kmVec4 *buffer_in, size_t n_items)
{

}
