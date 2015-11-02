/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/24/15, Steven
 * 
 **/
#include <MacTypes.h>
#include <kazmath/vec3.h>
#include <kazmath/quaternion.h>
#include "slsTransform.h"

//---------------------------------constructors---------------------------------------
slsTransform *sls_transform_init(slsTransform *self)
{
  self->priv = NULL;
  self->priv = calloc(1, sizeof(slsTransform_p));
  sls_checkmem(self->priv);

  return self;

  error:
  return self ?
         sls_transform_dtor(self) :
         NULL;

}

slsTransform *sls_transform_dtor(slsTransform *self)
{
  return self;
}

//---------------------------------setters---------------------------------------

slsTransform *sls_transform_set_position(slsTransform *self, kmVec3 const *position)
{
  return NULL;
}

slsTransform *sls_transform_set_scale(slsTransform *self, kmVec3 const *scale)
{
  return NULL;
}

slsTransform *sls_transform_set_rotation(slsTransform *self, kmQuaternion const *rotation)
{
  return NULL;
}

//---------------------------------getters---------------------------------------

kmVec3 sls_transform_get_position(slsTransform *self)
{
  kmVec3 result;
  return result;
}

kmVec3 sls_transform_get_scale(slsTransform *self)
{
  kmVec3 result;
  return result;
}

kmQuaternion sls_transform_get_rotation(slsTransform *self)
{
  kmQuaternion result;
  return result;
}

void sls_transform_build_matrix(slsTransform *self)
{

}
