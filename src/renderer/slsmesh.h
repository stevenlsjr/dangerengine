/**
 * @file slsmesh.h
 * @brief
 *
 * Copyright (c) 6/23/15, Steven Shea
 * All rights reserved.
 **/

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

#include "../data-types/array.h"
#include "../sls-gl.h"
#include "slsutils.h"
#include "slsshader.h"
#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>

typedef struct slsMesh slsMesh;
typedef struct slsMesh_p slsMesh_p;

typedef struct slsVertex slsVertex;
typedef struct slsVertex2D slsVertex2D;

struct slsVertex {
  float position[3];
  float normal[3];
  float uv[2];
  float color[4];
};

struct slsVertex2D {
  float position[3];
  float uv[2];
  float color[4];
};

slsVertex sls_vertex_make(kmVec3 position,
                          kmVec3 normal,
                          kmVec2 uv,
                          kmVec4 color);

struct slsMesh {
  struct {
    slsVertex* data;
    size_t length;
  } vertices;

  struct {
    uint32_t* data;
    size_t length;
  } indices;

  GLuint vbo, ibo;
  GLuint vao;

  GLenum gl_draw_mode;
};

slsMesh const* sls_mesh_class();

slsMesh* sls_mesh_square(slsMesh* self_uninit);

slsVertex* sls_sphere_vertices(size_t n_vertices, kmVec4 const* color);

slsMesh* sls_sphere_mesh(size_t n_vertices, kmVec4 const* color);

slsMesh* sls_tile_mesh(size_t width, size_t height);

slsMesh* sls_mesh_init(slsMesh* self,
                       slsVertex const* vertices,
                       size_t vert_count,
                       unsigned const* indices,
                       size_t idx_count);

slsMesh* sls_mesh_dtor(slsMesh* self);

void sls_mesh_setup_buffers(slsMesh* self, slsShader* shader);

static inline slsMesh* sls_mesh_new(slsVertex const* vertices,
                                    size_t vert_count,
                                    unsigned const* indices,
                                    size_t idx_count)
{
  slsMesh* obj = (slsMesh*)sls_objalloc(sls_mesh_class(), sizeof(slsMesh));
  return sls_mesh_init(obj, vertices, vert_count, indices, idx_count);
}

static inline void sls_mesh_delete(slsMesh* self)
{
  if (self) {
    slsMesh* zombie = sls_mesh_dtor(self);
    if (zombie) {
      free(zombie);
    }
  } else {
    assert(!"invalid plane_mesh instance!");
  }
}

/**
 * binds buffer and vertex resources to OpenGL state
 * @param self
 */
void sls_mesh_bindbuffers(slsMesh* self) SLS_NONNULL(1);

/**
 * unbinds buffer resources
 */
void sls_mesh_unbind();

#endif // DANGERENGINE_SLS_MESH_H
