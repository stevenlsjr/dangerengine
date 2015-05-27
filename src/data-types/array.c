//
// Created by Steven on 5/3/15.
//

#include "array.h"
#include "../slsutils.h"
#include "../math/mathmacs.h"


slsArray *sls_array_init(slsArray *self, void *data, size_t element_size, size_t n_elements);

void sls_array_dtor(slsArray *self);

size_t sls_array_length(slsArray *self);

size_t sls_array_element_size(slsArray *self);



void *sls_array_get(slsArray *self, size_t i);

struct slsArray_p {
  size_t length;
  size_t alloc_size;
  size_t element_size;
  char *array;
};

static const slsArray sls_array_proto = {
  .init = sls_array_init,
  .dtor = sls_array_dtor,
  .length = sls_array_length,
  .get = sls_array_get,
  .priv = NULL
};

slsArray const *sls_array_class()
{
  return &sls_array_proto;
}

slsArray *sls_array_new(void *array, size_t element_size, size_t n_elements)
{
  slsArray *self = NULL;
  self = sls_objalloc(sls_array_class(), sizeof(slsArray));
  if (self) {
    self = sls_msg(self, init, array, element_size, n_elements);
  }
  return self;
}



slsArray *sls_array_init(slsArray *self,
                         void *data,
                         size_t element_size,
                         size_t n_elements)
{
  if (!self || !data || (element_size == 0)) {
    return NULL;
  }

  self->priv = calloc(1, sizeof(slsArray_p));
  sls_checkmem(self->priv);
  size_t const alloc_size = sls_nearest_squarelu(element_size * n_elements);

  *(self->priv) = (slsArray_p){
    .length = n_elements,
    .element_size = element_size,
    .alloc_size = alloc_size,
    .array = calloc(alloc_size, sizeof(char))};



  return self;

error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  return NULL;
}

size_t sls_array_length(slsArray *self)
{
  return 0;
}

void sls_array_dtor(slsArray *self)
{

}

size_t sls_array_element_size(slsArray *self)
{
  return 0;
}

void *sls_array_get(slsArray *self, size_t i)
{
  if (!self) {return NULL;}
  sls_check(i < self->priv->length, "out of index error!");

  char *ptr = self->priv->array + (i * self->priv->element_size);

  return ptr;

error:
  return NULL;
}