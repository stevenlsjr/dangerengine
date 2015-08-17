//
// Created by Steven on 8/6/15.
//

#ifndef DANGERENGINE_SLSENTITY_H
#define DANGERENGINE_SLSENTITY_H

typedef enum slsComponentMask {
  SLS_COMPONENT_NONE = 0,
  SLS_COMPONENT_NAME = 1 << 1,
  SLS_COMPONENT_TRANSFORM = 1 << 2,
  SLS_COMPONENT_MODEL = 1 << 3,
  SLS_COMPONENT_TEXTURE = 1 << 4
} slsComponentMask;

typedef struct slsTransform slsTransform;

typedef struct slsEntity slsEntity;

struct slsEntity {
  slsEntity *(*init)(slsEntity *self);

  slsEntity *(*dtor)(slsEntity *self);

  slsPtrArray *children;

  slsComponentMask component_mask;
  char *name;
  slsModel *model;
  slsTexture *texture;
};


#endif //DANGERENGINE_SLSENTITY_H
