//
// Created by Steven on 8/16/15.
//

#include <kazmath/mat4.h>
#include <kazmath/vec2.h>
#include <stdlib.h>
#include <state/slsEntity.h>
#include "slsTransform2D.h"

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out,
                                  slsTransform2D const *in)
{
  if (!out || !in) { return NULL; }

  kmMat4 tmp, translate, rotate, scale;
  kmMat4Identity(out);

#if 1
  kmMat4RotationZ(&rotate, in->rot);

  kmMat4Scaling(&scale, in->scale.x, in->scale.y, 1.0f);

  tmp = *out;
  kmMat4Multiply(out, &scale, &rotate);

  kmMat4Translation(&translate, in->pos.x, in->pos.y, (float)in->z_layer);

  tmp = *out;
  kmMat4Multiply(out, &translate, &tmp);


#endif
  return out;
}

slsBool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b)
{
  sls_check(a, "a is NULL");
  sls_check(b, "b is NULL");

  return kmVec2AreEqual(&a->pos, &b->pos) &&
         kmVec2AreEqual(&a->scale, &b->scale) &&
         a->rot == b->rot;

  error:
  return SLS_FALSE;
}

slsBool sls_transform2D_near(slsTransform2D const *a, slsTransform2D const *b, float epsilon)
{
  
  return SLS_TRUE;
}

slsTransform2D *sls_transform2d_init(slsTransform2D *self)
{
  self->pos = (kmVec2) {0, 0};
  self->rot = 0;
  self->scale = (kmVec2) {1, 1};
  self->z_layer = 0;
  return self;
}

slsTransform2D sls_transform2d_local2world(slsEntity *entity)
{
  slsTransform2D a = entity->transform;
  slsTransform2D b;


  for (slsEntity *p = entity->parent; p; p = p->parent) {
    b = a;
    kmVec2Add(&a.pos, &p->transform.pos, &b.pos);
    kmVec2Mul(&a.scale, &p->transform.scale, &b.scale);

    a.rot = p->transform.rot + b.rot;
  }



  return a;
}
