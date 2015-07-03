//
// Created by Steven on 7/2/15.
//

#ifndef DANGERENGINE_SLSMANAGER_H
#define DANGERENGINE_SLSMANAGER_H

#include "../data-types/hashtable.h"

typedef struct slsManager slsManager;

struct slsManager {

  slsManager *(*init)(slsManager *self);
  void (*dtor)(slsManager *self);

  slsHashtable *meshes;
  slsHashtable *textures;
  slsHashtable *shaders;
};


#endif //DANGERENGINE_SLSMANAGER_H
