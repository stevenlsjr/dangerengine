/**
 * Created by Steven on 6/23/15.
 * @file
 * @brief
 */

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

#include "../data-types/array.h"
#include "../sls-gl.h"

typedef struct slsMesh slsMesh;
typedef struct slsMesh_p slsMesh_p;

typedef struct slsVertex slsVertex;



struct slsVertex {
  float position[3];
  float normal[3];
  float uv[2];
  float color[4];
};

struct slsMesh {
  slsMesh *(*init)(slsMesh *self,
                   slsVertex const *vertices,
                   size_t vert_count,
                   unsigned const *indices,
                   size_t idx_count);

  void (*dtor)(slsMesh *self);
  void (*bind)(slsMesh *self, GLuint shader_program);


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

  slsMesh_p *priv;


  void *data;
};

slsMesh const *sls_mesh_class();

slsMesh *sls_mesh_new(slsVertex const *vertices,
                      size_t vert_count,
                      unsigned const *indices,
                      size_t idx_count);


#endif //DANGERENGINE_SLS_MESH_H

