/**
 * @file slsmesh.h
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

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

#include "../data-types/array.h"
#include "../sls-gl.h"
#include "data-types/slsutils.h"
#include "slsshader.h"
#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>

typedef struct slsMesh slsMesh;
typedef struct slsMesh_p slsMesh_p;

typedef struct slsVertex slsVertex;

struct slsVertex {
  float position[3];
  float normal[3];
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
