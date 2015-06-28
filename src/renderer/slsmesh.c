//
// Created by Steven on 6/23/15.
//

#include "slsmesh.h"
#include "../slsutils.h"
#include "shaderutils.h"
#include "string.h"

#include <assert.h>

slsMesh *sls_mesh_init(slsMesh *self,
                       slsVertex const *vertices,
                       size_t vert_count,
                       unsigned const *indices,
                       size_t idx_count);

void sls_mesh_bind_buffer_data(slsMesh *pMesh, GLuint program);
void sls_mesh_bind_attributes(slsMesh *pMesh, GLuint program);

void sls_mesh_dtor(slsMesh *self);

void sls_mesh_bind(slsMesh *self, GLuint shader_program);


struct slsMesh_p {
  int placeholder;
};


static const slsMesh sls_mesh_proto = {
  .init = sls_mesh_init,
  .dtor = sls_mesh_dtor,
  .bind = sls_mesh_bind,
  .vbo = 0,
  .ibo = 0,
  .vao = 0,
};

/*================================
 * IMPLEMENTATIONS
 *================================*/

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

  glGenVertexArrays(1, &self->vao);
  glGenBuffers(1, &self->vbo);
  glGenBuffers(1, &self->ibo);

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

void sls_mesh_bind(slsMesh *self, GLuint shader_program)
{
  if (!self) { return; }


  // bind gl objects
  glUseProgram(shader_program);
  glBindVertexArray(self->vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  sls_mesh_bind_buffer_data(self, shader_program);
  sls_mesh_bind_attributes(self, shader_program);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void sls_mesh_bind_buffer_data(slsMesh *self, GLuint program)
{
  if (!self) { return; }


  const size_t vbo_size = sls_msg(self->vertices, length) * sls_msg(self->vertices, element_size);
  const size_t ibo_size = sls_msg(self->indices, length) * sls_msg(self->indices, element_size);

  assert(sls_msg(self->indices, element_size) == sizeof(unsigned int));
  assert(sls_msg(self->vertices, element_size) == sizeof(slsVertex));

  slsVertex *verts = sls_msg(self->vertices, get, 0);
  unsigned int *idxs = sls_msg(self->indices, get, 0);

  // push index buffer data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, idxs, GL_STATIC_DRAW);

  // push vertex buffer data
  glBufferData(GL_ARRAY_BUFFER, vbo_size, verts, GL_STATIC_DRAW);

}


void sls_mesh_bind_attributes(slsMesh *self, GLuint program)
{


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

  if (!self) {return;}

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
      (slsVertex){.position={-1.0, -1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={0.0, 0.0}, .color={1.0, 1.0, 0.0, 1.0}},
      (slsVertex){.position={-1.0,  1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={0.0, 1.0}, .color={0.0, 1.0, 1.0, 1.0}},
      (slsVertex){.position={ 1.0,  1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={1.0, 1.0}, .color={1.0, 0.0, 1.0, 1.0}},
      (slsVertex){.position={ 1.0,  -1.0, 0.0}, .normal={0.0, 0.0, 1.0}, .uv={1.0, 0.0}, .color={1.0, 0.0, 1.0, 1.0}}
    };

    unsigned int idxs[] = { 0, 1, 2, 3, 2, 0 };

    size_t n_verts = sizeof(verts) / sizeof(slsVertex);
    size_t n_idx = sizeof(idxs) / sizeof(unsigned int);

    mesh = sls_mesh_new(verts, n_verts, idxs, n_idx);

  } else {
    sls_log_err("invalid name given");
  }

  return mesh;

}