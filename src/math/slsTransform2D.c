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

kmVec2 sls_transform2d_local_to_world(slsTransform2D *self,
                                      slsMatrixStack *mat_stack,
                                      kmVec2 const *in_opt)
{

  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;

  mat_stack->n_matrices = 0;
  kmMat4 tmpmat;

  sls_transform2d_modelview(self, mat_stack, &tmpmat);

  kmVec4MultiplyMat4(&result, &res_in, &tmpmat);

  return (kmVec2) {result.x, result.y};
}


kmVec2 sls_transform2d_world_to_local(slsTransform2D *self,
                                      slsMatrixStack *mat_stack,
                                      kmVec2 const *in_opt)
{
  kmVec4 result = (in_opt) ?
                  (kmVec4) {in_opt->x, in_opt->y, 0.0, 1.0} :
                  (kmVec4) {0.0, 0.0, 0.0, 1.0};
  kmVec4 res_in = result;

  mat_stack->n_matrices = 0;
  kmMat4 tmpmat, tmpmat1;

  sls_transform2d_modelview(self, mat_stack, &tmpmat);


  tmpmat1 = tmpmat;


  kmVec4MultiplyMat4(&result, &res_in, &tmpmat);

  return (kmVec2) {result.x, result.y};
}

kmMat4 *sls_transform2d_modelview(slsTransform2D *self, slsMatrixStack *mat_stack, kmMat4 *out)
{
  mat_stack->n_matrices = 0;
  kmMat4 pushed_mat, tmpmat;


  for (slsEntity *e = self->entity; e != NULL; e = e->parent) {
    sls_matrix_stack_push(mat_stack, sls_transform2D_to_matrix(&tmpmat, &e->transform));
  }

  kmMat4Identity(out);
  while (mat_stack->n_matrices > 0) {
    kmMat4 mul_in = *out;
    sls_matrix_stack_pop(mat_stack, &pushed_mat);
    kmMat4Multiply(out, &mul_in, &pushed_mat);
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
