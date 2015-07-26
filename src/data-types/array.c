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


struct slsArray_p {
  size_t length;
  size_t alloc_size;
  size_t element_size;
  char *array;
};

static const slsArray sls_array_proto = {
  .init = sls_array_init,
  .dtor = sls_array_dtor,
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
  sls_check_code(i <= self->priv->length, SLS_INDEX_OVERFLOW, "out of index error!");

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

void sls_array_set(slsArray *self, size_t i, void *value)
{

  void *ptr = (void*)sls_array_get(self, i);
  if (!ptr || !value) {return;}

  memcpy(ptr, value, self->priv->element_size);
}

void sls_array_insert(slsArray *self, size_t i, void *value)
{
  if (!self || ! self->priv) { return;}
  slsArray_p *p = self->priv;
  size_t len = p->length;
  size_t newlen = len + 1;
  size_t newsize = newlen * p->element_size;
  sls_check_code(i <= len,
                 SLS_INDEX_OVERFLOW,
                 "index %lu cannot be appended above %lu", i, len);

  // resize array if necessary
  if (newsize >= p->alloc_size) {
    newsize = p->alloc_size * 2;
    p->array = realloc(p->array, newsize);
    sls_checkmem(p->array);
    p->alloc_size = newsize;
  }

  // now shift bytes determined by offset size
  size_t items_offset = len - i;
  if (items_offset > 0) {
    size_t byteidx = i * p->element_size;
    char *src = p->array + i * p->element_size; // location which inserted value is offseting
    char *offset = p->array + (i + 1) * p->element_size; // new location of bytes
    memmove(offset, src, p->element_size * items_offset);
  }

  // set value and increment length
  sls_array_set(self, i, value);
  ++p->length;

  return;
error:
  return;
}


size_t sls_array_alloc_size(slsArray *self)
{
  return self->priv->alloc_size;
}

void sls_array_reserve(slsArray *self, size_t count)
{
  assert(self && self->priv);
  slsArray_p *p = self->priv;

  if (p->alloc_size < count) {
    size_t new_size = sls_nearest_squarelu(count);
    p->alloc_size = new_size;

    p->array = realloc(p->array, new_size * p->element_size);
    sls_checkmem(p->array);
  }

  return;
error:
  assert(0);
  return;

}
