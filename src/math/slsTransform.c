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


  kmMat4 ident;
  kmMat4Identity(&ident);
  sls_transform_set_modelview(self, &ident);

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


kmMat4 const *sls_transform_get_modelview(slsTransform *self)
{

  return &self->priv.__modelview;
}

kmMat4 const *sls_transform_get_inverse_modelview(slsTransform *self)
{

  return &self->priv.__inverse_modelview;

}

kmMat4 const *sls_transform_get_normalview(slsTransform *self)
{

  return &self->priv.__normalview;
}

kmMat4 const *sls_transform_set_modelview(slsTransform *self, kmMat4 const *mv)
{
  slsTransform_p *priv = &self->priv;
  priv->__modelview = *mv;
  priv->__inverse_modelview = *kmMat4Inverse(&priv->__inverse_modelview, mv);
  priv->__normalview = *kmMat4Transpose(&priv->__normalview, &priv->__inverse_modelview);

  return &priv->__modelview;
}

kmVec4 sls_transform_scale(slsTransform const *self, kmQuaternion *out)
{
  assert(!"not implemented");
  return (kmVec4){};
}

kmVec4 sls_transform_position(slsTransform const *self)
{
  kmVec4 res, in = {0.0, 0.0, 0.0, 1.0};
  kmVec4MultiplyMat4(&res, &in, &self->priv.__modelview);
  return res;
}

kmQuaternion *sls_transform_rotation(slsTransform const *self, kmQuaternion *out)
{
  assert(!"not implemented");

  return NULL;
}
