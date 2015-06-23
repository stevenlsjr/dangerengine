//
// Created by Steven on 6/23/15.
//

#ifndef DANGERENGINE_SLS_MESH_H
#define DANGERENGINE_SLS_MESH_H

typedef struct slsMesh slsMesh;
typedef struct slsMesh_p slsMesh_p;

struct slsMesh {
  slsMesh *(*init)();
  void (*dtor)();



  slsMesh_p *priv;
  void *data;
};

#endif //DANGERENGINE_SLS_MESH_H
