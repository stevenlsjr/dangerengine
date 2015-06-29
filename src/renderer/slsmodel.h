#ifndef SLS_MODEL_H
#define SLS_MODEL_H

#include "../sls-gl.h"
#include <kazmath/kazmath.h>

typedef struct kmMat4 kmMat4;
typedef struct slsMesh slsMesh;
typedef struct slsModel slsModel;
typedef struct slsModel_p slsModel_p;

struct slsModel
{
  slsModel*(*init)(slsModel* self,
                   slsMesh* mesh,
                   GLuint program,
                   kmMat4 const* transform);
  void (*dtor)(slsModel* self);
  /**
  * @brief pushes current transform to the model_view matrix stack
  * 
  * @param model_view current model_view matrix.
  * @param model_view_handle shader index for model_view matrix
  * @return composition of model transform and current model_view
  */
  kmMat4 (*push_transform)(slsModel* self,
                           kmMat4 const* model_view,
                           GLuint model_view_handle);

  void (*draw)(slsModel* self,
               GLenum draw_type,
               double dt);

  slsMesh* mesh;
  GLuint program;
  kmMat4 transform;

  slsModel_p* priv;
  void* data;
};

slsModel const* sls_model_class();
slsModel* sls_model_new(slsMesh* mesh,
                        GLuint program,
                        kmMat4 const* transform);


#endif // SLS_MODEL_H


