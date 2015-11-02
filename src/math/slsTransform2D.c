//
// Created by Steven on 8/16/15.
//

#include <kazmath/mat4.h>
#include <kazmath/vec2.h>
#include <stdlib.h>
#include <state/slsEntity.h>
#include "slsTransform2D.h"

kmMat4 *sls_transform2D_to_matrix(kmMat4 *out,
                                  slsTransform2D *in)
{
  if (!out || !in) { return NULL; }

  kmMat4 tmp, translate, rotate, scale;
  kmMat4Identity(out);

  kmMat4RotationZ(&rotate, in->rot);

  kmMat4Scaling(&scale, in->scale.x, in->scale.y, 1.0f);

  tmp = *out;
  kmMat4Multiply(out, &scale, &rotate);

  kmMat4Translation(&translate, in->pos.x, in->pos.y, (float) in->z_layer);

  tmp = *out;
  kmMat4Multiply(out, &translate, &tmp);


  in->local_matrix = *out;


  return out;
}

bool sls_transform2D_eq(slsTransform2D const *a, slsTransform2D const *b)
{
  sls_check(a, "a is NULL");
  sls_check(b, "b is NULL");

  return kmVec2AreEqual(&a->pos, &b->pos) &&
         kmVec2AreEqual(&a->scale, &b->scale) &&
         a->rot == b->rot;

  error:
  return SLS_FALSE;
}

bool sls_transform2D_near(slsTransform2D const *a, slsTransform2D const *b, float epsilon)
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

kmVec2 sls_transform2d_local_to_world(slsTransform2D *self, kmVec2 const *in_opt)
{

  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;

  kmMat4 tmpmat;

  sls_transform2d_modelview(self, &tmpmat);

  kmVec4MultiplyMat4(&result, &res_in, &tmpmat);

  return (kmVec2) {result.x, result.y};
}


kmVec2 sls_transform2d_world_to_local(slsTransform2D *self, kmVec2 const *in_opt)
{
  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;

  kmMat4 tmpmat, tmpmat_b;

  sls_transform2d_modelview(self, &tmpmat_b);


  kmMat4Inverse(&tmpmat, &tmpmat_b);


  kmVec4MultiplyMat4(&result, &res_in, &tmpmat);

  return (kmVec2) {result.x, result.y};
}

kmMat4 *sls_transform2d_modelview(slsTransform2D *self, kmMat4 *out)
{
  kmMat4 tmpmat;

  kmMat4Identity(out);

  for (slsEntity *e = self->entity; e != NULL; e = e->parent) {
    kmMat4 mul_in = *out;
    kmMat4Multiply(out, sls_transform2D_to_matrix(&tmpmat, &e->transform), &mul_in);

  }



  self->model_view = *out;

  return out;
}

float sls_transform2d_local_to_world_angle(slsTransform2D *self, float angle)
{
  for (slsEntity *e = self->entity; e != NULL; e = e->parent) {
    angle += e->transform.rot;
  }
  return angle;
}


float sls_transform2d_world_to_local_angle(slsTransform2D *self, float angle)
{
  for (slsEntity *e = self->entity; e != NULL; e = e->parent) {
    angle -= e->transform.rot;
  }
  return angle;
}
