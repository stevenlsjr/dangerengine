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

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

#include "../data-types/array.h"
#include "../sls-gl.h"
#include "../slsutils.h"
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
  slsMesh *(*init)(slsMesh *self,
                   slsVertex const *vertices,
                   size_t vert_count,
                   unsigned const *indices,
                   size_t idx_count);

  void (*dtor)(slsMesh *self);

  void (*bind)(slsMesh *self, slsShader *shader_program);


  /**
   * @brief set up buffer objects for drawing
   */
  void(*pre_draw)(slsMesh *self, GLuint program, double dt);

  /**
   * @brief calls drawElements routine
   */
  void(*draw)(slsMesh *self, double dt);

  /**
   * @brief unbinds buffer objects
   */
  void(*post_draw)(slsMesh *self, GLuint program, double dt);

  /**
   * @brief vertex array
   * @details TYPE slsVertex
   */
  slsArray *vertices;

  /**
   * @brief index array
   * @details TYPE unsigned
   */
  slsArray *indices;

  GLuint vbo, ibo;
  GLuint vao;

  /**
  * @brief true if pre_draw has been called
  * @defails false if post_draw has unbound mesh or pre_draw hasn't been set up yet
  */
  slsBool is_drawing;

  slsMesh_p *priv;

  void *data;
};

slsMesh const *sls_mesh_class();

slsMesh *sls_mesh_new(slsVertex const *vertices,
                      size_t vert_count,
                      unsigned const *indices,
                      size_t idx_count);


slsMesh *sls_mesh_create_shape(char const *name);

slsMesh * sls_mesh_square();


void _sls_mesh_roughdraw(slsMesh *self, GLuint program, double dt);

slsVertex *sls_sphere_vertices(size_t n_vertices,
                               kmVec4 const *color);

slsMesh *sls_sphere_mesh(size_t n_vertices,
                         kmVec4 const *color);

#endif //DANGERENGINE_SLS_MESH_H

