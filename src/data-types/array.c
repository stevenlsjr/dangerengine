//
// Created by Steven on 5/3/15.
//

#include "array.h"
#include "../slsutils.h"
#include "../math/mathmacs.h"
#include <stdlib.h>
#include <string.h>


slsArray *sls_array_init(slsArray *self,
                         void const *data,
                         size_t element_size,
                         size_t n_elements);

void sls_array_dtor(slsArray *self);

size_t sls_array_length(slsArray const *self);

size_t sls_array_element_size(slsArray const *self);


slsArray *sls_array_copy(slsArray const *self);


void const *sls_array_get(slsArray const *self, size_t i);

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
  .element_size = sls_array_element_size,
  .copy = sls_array_copy,
  .get = sls_array_get,
  .priv = NULL
};

slsArray const *sls_array_class()
{
  return &sls_array_proto;
}

slsArray *sls_array_new(void const *array, size_t element_size, size_t n_elements)
{
  slsArray *self = NULL;
  self = sls_objalloc(sls_array_class(), sizeof(slsArray));
  if (self) {
    self = sls_msg(self, init, array, element_size, n_elements);
  }
  return self;
}


slsArray *sls_array_init(slsArray *self,
                         void const *data,
                         size_t element_size,
                         size_t n_elements)
{
  if (!self || !data || (element_size == 0)) {
    return NULL;
  }

  self->priv = calloc(1, sizeof(slsArray_p));
  sls_checkmem(self->priv);

  size_t const alloc_size =
    sls_nearest_squarelu(element_size * n_elements);

  size_t const element_bytes = element_size * n_elements;

  *(self->priv) = (slsArray_p) {
    .length = n_elements,
    .element_size = element_size,
    .alloc_size = alloc_size,
    .array = calloc(alloc_size, sizeof(char))
  };

  sls_checkmem(self->priv->array);

  // copy array to buffer
  sls_check(memcpy(self->priv->array,
                   data,
                   element_bytes),
            "slsArray::init-> array failed to copy to buffer!");


  return self;

  error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  return NULL;
}

size_t sls_array_length(slsArray const *self)
{
  sls_check(self && self->priv, "null pointer");
  return self->priv->length;

error:
  return 0;
}

void sls_array_dtor(slsArray *self)
{
  if (!self) { return; }
  if (self->priv) {
    if (self->priv->array) {
      free(self->priv->array);
    }
    free(self->priv);
  }

  free(self);
}

size_t sls_array_element_size(slsArray const *self)
{
  sls_check(self && self->priv, "null pointer");

  return self->priv->element_size;
error:
  return 0;
}

void const *sls_array_get(slsArray const *self, size_t i)
{
  if (!self) { return NULL; }
  sls_check(i <= self->priv->length, "out of index error!");

  char *ptr = self->priv->array + (i * self->priv->element_size);

  return ptr;

  error:
  return NULL;
}

slsArray *sls_array_copy(slsArray const *self)
{
  return sls_array_new(self->priv->array,
                       self->priv->element_size,
                       self->priv->length);
}