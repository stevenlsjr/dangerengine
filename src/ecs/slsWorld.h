//
// Created by Steven on 7/22/15.
//

#ifndef DANGERENGINE_SLSWORLD_H
#define DANGERENGINE_SLSWORLD_H

#include <renderer/slsmodel.h>

#define SLS_ENTITIES_PER_LIST 640

typedef enum slsComponentMask {
  SLS_COMPONENT_MODEL
} slsComponentMask;

typedef struct slsEntityList slsEntityList;

struct slsEntityList {
  slsComponentMask masks[SLS_ENTITIES_PER_LIST];
  slsModel *models[SLS_ENTITIES_PER_LIST];
};

#endif //DANGERENGINE_SLSWORLD_H
