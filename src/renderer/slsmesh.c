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
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#include "../slsutils.h"
#include "shaderutils.h"
#include "slsmesh.h"

static const slsMesh sls_mesh_proto = {.vbo = 0,
                                       .ibo = 0,
                                       .vao = 0,
                                       .gl_draw_mode = GL_TRIANGLES };

/*================================
 * IMPLEMENTATIONS
 *================================*/

slsVertex sls_vertex_make(kmVec3 position, kmVec3 normal, kmVec2 uv,
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

slsMesh *sls_mesh_init(slsMesh *self, slsVertex const *vertices,
                       size_t vert_count, unsigned const *indices,
                       size_t idx_count)
{
  if (!vertices || !indices || !self) {
    return NULL;
  }
  *self = sls_mesh_proto;

  self->vertices.data = calloc((vert_count + 1), sizeof(slsVertex));
  self->indices.data = calloc((idx_count + 1), sizeof(uint32_t));

  memcpy(self->vertices.data, vertices, vert_count * sizeof(slsVertex));
  memcpy(self->indices.data, indices, idx_count * sizeof(uint32_t));
  self->vertices.length = vert_count;
  self->indices.length = idx_count;


  glGenBuffers(1, &self->vbo);
  glGenBuffers(1, &self->ibo);

  glGenVertexArrays(1, &self->vao);

  return self;
error:
  sls_mesh_dtor(self);
  return NULL;
}

slsMesh *sls_mesh_dtor(slsMesh *self)
{
  if (!self) {
    return NULL;
  }

  GLuint buffers[] = {self->vbo, self->ibo};

  if (self->vertices.data) {
    free(self->vertices.data);
  }

  if (self->indices.data) {
    free(self->indices.data);
  }

  if (glIsBuffer(buffers[0])) {
    glDeleteBuffers(sizeof(buffers) / sizeof(GLuint), buffers);
  }

  glDeleteVertexArrays(1, &(self->vao));

  return self;
}

void sls_mesh_bind(slsMesh *self, slsShader *shader)
{
#ifndef SLS_GNU_EXT
  if (!self) {
    return;
  }
#endif

  // bind gl objects
  glUseProgram(shader->program);

  glBindVertexArray(self->vao);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  _sls_mesh_binddata(self, shader->program);

  _sls_mesh_bindattrs(self, shader->program);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void _sls_mesh_binddata(slsMesh *self, GLuint program)
{
  if (!self) {
    return;
  }
  // bind gl objects
  glUseProgram(program);
#ifndef SLS_GLES
  glBindVertexArray(self->vao);
#endif

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  const size_t vbo_size = sizeof(slsVertex) * self->vertices.length;
  const size_t ibo_size = sizeof(uint32_t) * self->indices.length;

  slsVertex const *verts = self->vertices.data;
  unsigned int const *idxs = self->indices.data;

  // push index buffer data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, idxs, GL_STATIC_DRAW);

  // push vertex buffer data
  glBufferData(GL_ARRAY_BUFFER, vbo_size, verts, GL_DYNAMIC_DRAW);
}

void _sls_mesh_bindattrs(slsMesh *self, GLuint program)
{

  if (!self) {
    return;
  }

  glUseProgram(program);
#ifndef SLS_GLES
  glBindVertexArray(self->vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  glVertexAttribPointer(SLS_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *)offsetof(slsVertex, position));
  glEnableVertexAttribArray(SLS_ATTRIB_POSITION);

  glVertexAttribPointer(SLS_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *)offsetof(slsVertex, normal));
  glEnableVertexAttribArray(SLS_ATTRIB_NORMAL);

  glVertexAttribPointer(SLS_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(slsVertex),
                        (GLvoid *)offsetof(slsVertex, uv));
  glEnableVertexAttribArray(SLS_ATTRIB_UV);

  glVertexAttribPointer(SLS_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid *)offsetof(slsVertex, color));
  glEnableVertexAttribArray(SLS_ATTRIB_COLOR);
}

void sls_mesh_draw(slsMesh *self)
{
  // setup vert position pointer
  glBindVertexArray(self->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  size_t elements = self->indices.length;

  glDrawElements(self->gl_draw_mode, (int)elements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sls_mesh_update_verts(slsMesh *self, slsShader *shader)
{
  glDeleteBuffers(2, (GLuint[]){ self->ibo, self->vbo });
  glDeleteVertexArrays(1, &self->vao);

  glGenBuffers(1, &self->vbo);
  glGenBuffers(1, &self->ibo);
  glGenVertexArrays(1, &self->vao);

  sls_mesh_bind(self, shader);
}

//---------------------------------plane_mesh
// generations---------------------------------------

slsVertex *sls_sphere_vertices(size_t n_vertices, kmVec4 const *color)
{
  slsVertex *sphere = NULL;

  size_t sphere_size = n_vertices + 1;
  sphere = calloc(sphere_size, sizeof(slsVertex));

  for (int i = 0; i < n_vertices; ++i) {
    double theta = (M_PI * 2.0 * (double)i) / (double)n_vertices;

    float pos[3] = { (float)cos(theta), (float)sin(theta), 1.0f };

    slsVertex v = {.normal = {0.0, 0.0, 1.0}, .uv = {0.0, 0.0}};
    memcpy(v.position, pos, sizeof(float[3]));
    v.color[0] = color->x;
    v.color[1] = color->y;
    v.color[2] = color->z;
    v.color[3] = color->w;
  }

  return sphere;
}

slsMesh *sls_sphere_mesh(size_t n_vertices, kmVec4 const *color)
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
    uint32_t triangle[3] = { 0, (uint32_t)i, (uint32_t)i + 1 };

    memcpy(elements + i, triangle, sizeof(uint32_t[3]));
  }

  m = sls_mesh_new(verts, n_vertices, elements, n_elements);

  free(elements);
  free(verts);

  return m;
}

slsMesh *sls_mesh_square(slsMesh *self_uninit)
{
  slsVertex verts[] = { (slsVertex){.position = { -1.f, -1.f, 0.f },
                                    .normal = { 0.f, 0.f, 1.f },
                                    .uv = { 0.f, 0.f },
                                    .color = { 1.f, 1.f, 1.f, 1.f } },
                        (slsVertex){.position = { 1.f, -1.f, 0.f },
                                    .normal = { 0.f, 0.f, 1.f },
                                    .uv = { 1.f, 0.f },
                                    .color = { 1.f, 1.f, 1.f, 1.f } },
                        (slsVertex){.position = { 1.f, 1.f, 0.f },
                                    .normal = { 0.f, 0.f, 1.f },
                                    .uv = { 1.f, 1.f },
                                    .color = { 1.f, 1.f, 1.f, 1.f } },
                        (slsVertex){.position = { -1.f, 1.f, 0.f },
                                    .normal = { 0.f, 0.f, 1.f },
                                    .uv = { 0.f, 1.f },
                                    .color = { 1.f, 1.f, 1.f, 1.f } } };

  unsigned int idxs[] = {0, 1, 2, 3, 0, 2};

  size_t n_verts = sizeof(verts) / sizeof(slsVertex);
  size_t n_idx = sizeof(idxs) / sizeof(unsigned int);

  return sls_mesh_init(self_uninit, verts, n_verts, idxs, n_idx);
}
