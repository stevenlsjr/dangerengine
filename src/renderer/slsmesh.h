/**
 * Created by Steven on 6/23/15.
 * @file
 * @brief
 */

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

#include "../data-types/array.h"
#include "../sls-gl.h"
#include "../slsutils.h"
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
  void (*bind)(slsMesh *self, GLuint shader_program);

  void(*bind_buffers)(slsMesh *self, GLuint shader_program);
  void(*bind_attributes)(slsMesh *self, GLuint shader_program);

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

void _sls_mesh_roughdraw(slsMesh *self, GLuint program, double dt);

#endif //DANGERENGINE_SLS_MESH_H

