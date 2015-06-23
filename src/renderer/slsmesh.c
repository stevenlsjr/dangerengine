//
// Created by Steven on 6/23/15.
//

#include "slsmesh.h"
#include "../slsutils.h"

slsMesh *sls_mesh_init(slsMesh *self,
                       slsVertex const *vertices,
                       size_t vert_count,
                       unsigned const *indices,
                       size_t idx_count);

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
  if (!vertices || ! indices || !self) {return NULL;}
  self->indices = sls_array_new(indices, sizeof(unsigned), idx_count);
  self->vertices = sls_array_new(vertices, sizeof(slsVertex), vert_count);

  self->priv = calloc(sizeof(slsMesh_p), 1);
  sls_checkmem(self->priv);

  glGenVertexArrays(1, &self->vao);
  glGenBuffers(1, &self->vbo);
  glGenBuffers(1, &self->ibo);

  return self;
error:
  if (self && self->dtor) {sls_msg(self, dtor);}
  return NULL;
}

void sls_mesh_dtor(slsMesh *self)
{
  if(!self) {return;}

  if (self->priv) {free(self->priv);}

  sls_msg(self->indices, dtor);
  sls_msg(self->vertices, dtor);

  GLuint buffers[] = {self->vbo, self->ibo};


  glDeleteBuffers(sizeof(buffers)/sizeof(GLuint), buffers);
  glDeleteVertexArrays(1, &(self->vao));

  free(self);
}

void sls_mesh_bind(slsMesh *self, GLuint shader_program)
{
  if (!self) {return;}

  GLuint attrs =0;

  GLuint position, normal, uv, color;
  {
    // set proper layout locations
    position = attrs++;
    normal = attrs++;
    uv = attrs++;
    color = attrs++;
  }

  slsVertex *verts = sls_msg(self->vertices, get, 0);
  // bind gl objects
  glUseProgram(shader_program);
  glBindVertexArray(self->vao);
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ibo);

  // set buffer data
  const size_t vbo_size = sls_msg(self->vertices, length) * sls_msg(self->vertices, element_size);
  glBufferData(GL_ARRAY_BUFFER, vbo_size, verts, GL_STATIC_DRAW);



  glVertexAttribPointer(position,
                        3,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid*)offsetof(slsVertex, position));
  glEnableVertexAttribArray(position);

  glVertexAttribPointer(normal,
                        3,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid*)offsetof(slsVertex, normal));
  glEnableVertexAttribArray(normal);

  glVertexAttribPointer(uv,
                        2,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid*)offsetof(slsVertex, uv));
  glEnableVertexAttribArray(uv);


  glVertexAttribPointer(color,
                        4,
                        GL_FLOAT, GL_FALSE,
                        sizeof(slsVertex),
                        (GLvoid*)offsetof(slsVertex, color));
  glEnableVertexAttribArray(color);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}




