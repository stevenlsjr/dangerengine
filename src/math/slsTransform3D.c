#include "slsTransform3D.h"
#include "mathmacs.h"

/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/18/15, Steven
 * 
 **/

struct slsTransform3D_p {

  kmVec3 position;
  kmVec3 scale;
  kmQuaternion rotation;
  bool is_bound;
};

struct slsTransform3D *sls_transform3d_init(slsTransform3D *self)
{
  self->priv = calloc(1, sizeof(slsTransform3D_p));
  sls_checkmem(self->priv);

  sls_transform_init(&self->transform);

  // setup parameters
  {
    slsTransform3D_p *p = self->priv;
    p->position = (kmVec3){0.0, 0.0, 0.0};
    p->scale = (kmVec3){0.0, 0.0, 0.0};
    kmQuaternionIdentity(&p->rotation);
  }

  sls_transform3d_bind(self);

  assert(self->priv->is_bound);

  return self;

  error:
  if (self) {
    return sls_transform3d_dtor(self);
  } else {
    return NULL;
  }
}

struct slsTransform3D *sls_transform3d_dtor(slsTransform3D *self)
{
  if (self->priv) {
    free(self->priv);
  }
  sls_transform_dtor(&self->transform);
  return self;
}

bool sls_transform3d_is_bound(slsTransform3D *self)
{

  slsTransform3D_p *priv = self->priv;
  assert (priv);
  kmMat4 rot, scale, translate, acc;
  kmMat4RotationQuaternion(&rot, &priv->rotation);
  kmMat4Scaling(&scale, priv->scale.x, priv->scale.y, priv->scale.z);
  kmMat4Translation(&translate, priv->position.x, priv->position.y, priv->position.z);

  acc = sls_mat4_mul_valout(&scale, &rot);
  acc = sls_mat4_mul_valout(&translate, &acc);

  sls_transform_set_modelview(&self->transform, &acc);
  self->priv->is_bound = true;

  return self->priv->is_bound;


}

void sls_transform3d_bind(slsTransform3D *self)
{
  assert(self->priv);

}

kmVec3 const * sls_transform3d_position(slsTransform3D *self)
{
  assert(self->priv);

  return &self->priv->position;
}

kmVec3 const *sls_transform3d_scale(slsTransform3D *self)
{
  assert(self->priv);

  return &self->priv->scale;
}

kmQuaternion const *sls_transform3d_rotation(slsTransform3D *self)
{
  assert(self->priv);

  return &self->priv->rotation;
}

slsTransform3D *sls_transform3d_set_position(slsTransform3D *self, kmVec3 const *in)
{
  assert(self->priv && in);

  self->priv->is_bound = false;
  self->priv->position = *in;

  return self;
}

slsTransform3D *sls_transform3d_set_scale(slsTransform3D *self, kmVec3 const *in)
{
  assert(self->priv && in);
  self->priv->is_bound = false;
  self->priv->scale = *in;


  return self;
}

slsTransform3D *sls_transform3d_set_rotation(slsTransform3D *self, kmQuaternion const *in)
{
  assert(self->priv && in);

  self->priv->is_bound = false;
  self->priv->rotation = *in;

  return self;
}


