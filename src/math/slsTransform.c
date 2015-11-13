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

struct slsTransform_p {
  kmVec3 position;
  kmVec3 scale;
  kmQuaternion rotation;
};

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
  sls_checkmem(self->priv);
  self->priv->position = *position;
  return self;

  error:
  return self;
}

slsTransform *sls_transform_set_scale(slsTransform *self, kmVec3 const *scale)
{
  sls_checkmem(self->priv);
  self->priv->scale = *scale;

  return self;

  error:
  return self;
}

slsTransform *sls_transform_set_rotation(slsTransform *self, kmQuaternion const *rotation)
{
  sls_checkmem(self->priv);
  self->priv->rotation = *rotation;

  return self;

  error:
  return self;
}

//---------------------------------getters---------------------------------------

kmVec3 sls_transform_get_position(slsTransform *self)
{

  return self->priv->position;
}

kmVec3 sls_transform_get_scale(slsTransform *self)
{
  return self->priv->scale;
}

kmQuaternion sls_transform_get_rotation(slsTransform *self)
{
  return self->priv->rotation;
}

void sls_transform_build_matrix(slsTransform *self)
{

  slsTransform_p *p = self->priv;
  kmMat4 out, a, b;


  kmMat4RotationQuaternion(&a, &p->rotation);
  kmMat4Scaling(&b, p->scale.x, p->scale.y, p->scale.z);

  kmMat4Multiply(&out, &a, &b);
  b = out;
  kmMat4Translation(&a, p->position.x, p->position.y, p->position.z);


  kmMat4Multiply(&out, &a, &b);
  self->matrix = out;
}
