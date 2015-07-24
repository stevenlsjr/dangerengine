#include "slsmodel.h"
#include "../slsutils.h"
#include <kazmath/kazmath.h>
#include "slsmesh.h"

slsModel* sls_model_init(slsModel* self,
                         slsMesh* mesh,
                         GLuint program,
                         kmMat4 const* transform);
void sls_model_dtor(slsModel* self);

kmMat4 sls_model_push_transform(slsModel* self,
                                kmMat4 const* model_view,
                                GLuint model_view_handle);

void sls_model_draw(slsModel* self,
                    GLenum draw_type,
  double dt);


static const slsModel sls_model_proto = {
  .init = sls_model_init,
  .dtor = sls_model_dtor,
  .push_transform = sls_model_push_transform,
  .draw = sls_model_draw
};

slsModel const* sls_model_class()
{
  return &sls_model_proto;
}

slsModel* sls_model_new(slsMesh* mesh,
                        GLuint program,
                        kmMat4 const* transform)
{
  slsModel* self = sls_objalloc(sls_model_class(), sizeof(slsModel));
  return sls_msg(self, init, mesh, program, transform);
}

slsModel* sls_model_init(slsModel* self, slsMesh* mesh, GLuint program, kmMat4 const* transform)
{
  sls_check(self && mesh, 
            "invalid null arguments!! self: %lx mesh: %lx", 
            (size_t)self, (size_t)mesh);



  self->mesh = mesh;
  self->program = program;
  if (transform) {
    self->transform = *transform;
  } else {
    kmMat4Identity(&(self->transform));
  }

  return self;
error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  return NULL;
}

void sls_model_dtor(slsModel* self)
{
  if (!self) { return; }

  free(self);
}

kmMat4 sls_model_push_transform(slsModel* self, kmMat4 const* model_view, GLuint model_view_handle)
{
  if (!self) { 
    kmMat4 m;
    return *kmMat4Identity(&m);
  }
  kmMat4 new_mv;
  if (model_view) {
    kmMat4Multiply(&new_mv, model_view, &self->transform);
  } else {
    new_mv = self->transform;
  }

  glUseProgram(self->program);
  glUniformMatrix4fv(model_view_handle, 1, GL_FALSE, new_mv.mat);

  return new_mv;
}

void sls_model_draw(slsModel* self,
  GLenum draw_type,
  double dt)
{
  if (!self || !self->mesh) { return; }

  sls_msg(self->mesh, pre_draw, self->program, dt);
  sls_msg(self->mesh, draw, dt);
  sls_msg(self->mesh, post_draw, self->program, dt);

}