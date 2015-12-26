//
// Created by Steven on 8/16/15.
//

#include <kazmath/mat4.h>
#include <kazmath/vec2.h>
#include <stdlib.h>
#include <state/slsEntity.h>
#include "slsTransform2D.h"
#include <math/mathmacs.h>

kmMat4 *sls_transform2D_to_matrix(slsTransform2D *self, kmMat4 *out_opt)
{
  if (!out_opt || !self) { return NULL; }

  kmMat4 result, rot, scale, translate;
  kmMat4Translation(&translate, self->pos.x, self->pos.y, 0.0);
  kmMat4RotationZ(&rot, self->rot);
  kmMat4Scaling(&scale, self->scale.x, self->scale.y, 1.0);


  result = sls_mat4_mul_valout(&translate, kmMat4Multiply(&result, &scale, &rot));

  if (out_opt) {
    *out_opt = result;
  }

  sls_transform_set_modelview(&self->modelview, &result);

  return out_opt;
  error:
  return out_opt;
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

  assert(0);
  return false;
}

slsTransform2D *sls_transform2d_init(slsTransform2D *self)
{
  self->pos = (kmVec2) {0, 0};
  self->rot = 0;
  self->scale = (kmVec2) {1, 1};
  self->z_layer = 0;

  sls_transform_init(&self->modelview);
  sls_transform2D_to_matrix(self, NULL);


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
  sls_transform2D_to_matrix(self, NULL);


  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;


  kmMat4 tmpmat;

  sls_transform2d_modelview(self, &tmpmat);

  kmVec4MultiplyMat4(&result, &res_in, sls_transform_get_modelview(&self->modelview));

  return (kmVec2) {result.x, result.y};
}


kmVec2 sls_transform2d_world_to_local(slsTransform2D *self, kmVec2 const *in_opt)
{
  sls_transform2D_to_matrix(self, NULL);

  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;

  kmMat4 const *inverse = sls_transform_get_inverse_modelview(&self->modelview );
  kmVec4MultiplyMat4(&result, &res_in, inverse);
  return (kmVec2) {result.x, result.y};
}

kmMat4 *sls_transform2d_modelview(slsTransform2D *self, kmMat4 *out)
{
  kmMat4 tmpmat;

  sls_transform2D_to_matrix(self, NULL);


  return out;
}

float sls_transform2d_local_to_world_angle(slsTransform2D *self, float angle)
{
  return angle + self->rot;
}


float sls_transform2d_world_to_local_angle(slsTransform2D *self, float angle)
{

  return angle - self->rot;
}
