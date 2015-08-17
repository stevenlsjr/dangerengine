//
// Created by Steven on 7/22/15.
//

#include "ptrarray.h"


#include <math/mathmacs.h>

#include <slsutils.h>
#include <string.h>

#define SLS_PTRARRAY_SIZE_INC 2

slsPtrArray *sls_ptrarray_init(slsPtrArray *self,
                               void **data,
                               size_t n_elements,
                               slsFreeFn free_fn)
{
  assert(self);
  if (!self) { return NULL; }

  *self = (slsPtrArray) {
      .data = NULL,
      .free_fn = free_fn,
      .n_elements = n_elements
  };


  self->n_alloced = sls_nearest_squarelu(n_elements) * 2;

  assert(self->n_alloced >= self->n_elements * 2);

  self->data = calloc(self->n_alloced, sizeof(void *));
  sls_checkmem(self->data);

  // copy data to array buffer
  self->data = memcpy(self->data, data, n_elements * sizeof(void *));
  sls_checkmem(self->data);

  return self;

  error:
  if (self) {
    sls_ptrarray_dtor(self);
  }

  assert(0);
  return NULL;
}

slsPtrArray *sls_ptrarray_dtor(slsPtrArray *self)
{
  assert(self);
  if (!self) { return NULL; }

  if (self->data) {
    if (self->free_fn) {
      for (int i = 0; i < self->n_elements; ++i) {
        void *ptr = self->data[i];
        if (ptr) { self->free_fn(ptr); }
      }
    }

    free(self->data);
  }

  *self = (slsPtrArray) {
      .data=NULL,
      .free_fn=NULL,
      .n_elements=0,
      .n_alloced=0};

  return self;
}


void sls_ptrarray_reserve(slsPtrArray *self, size_t size)
{
  assert(self);
  if (!self) { return; }

  size_t new_size = sls_nearest_squarelu(size);

  if (new_size > self->n_alloced) {
    self->data = realloc(self->data, sizeof(void *) * new_size);
    sls_checkmem(self->data);
  }

  return;

  error:
  assert(0);
  return;
}

void sls_ptrarray_insert(slsPtrArray *self, size_t idx, void *ptr)
{
  assert(self && ptr);
  if (!self || !ptr) {return;}
  sls_check(self->data, "self->data does is null");

  sls_check_code(idx <= self->n_elements,
                 SLS_INDEX_OVERFLOW,
                 "Index overflow: index %lu is greater than array sized %lu",
                 idx, self->n_elements);


  if (self->n_elements + 1 >= self->n_alloced) {
    sls_ptrarray_reserve(self, self->n_alloced * SLS_PTRARRAY_SIZE_INC);
    assert(self->n_elements < self->n_alloced);
  }

  size_t offset = self->n_elements - idx;
  if (offset > 0) {
    sls_check(self->data[0], "self->data has no values");

    void **src = self->data + idx;
    void **dest = src + 1;
    void *res = memmove(dest, src, offset * sizeof(void *));

    sls_check(res == dest, "memmove failed! could not insert item into array");
  }


  self->data[idx] = ptr;
  self->n_elements++;

  return;

  error:
  assert(0);
}

void *sls_ptrarray_remove(slsPtrArray *self, size_t idx)
{
  return NULL;
}

void sls_ptrarray_append(slsPtrArray *self, void *ptr)
{
  sls_ptrarray_insert(self, self->n_elements, ptr);
}

void sls_ptrarray_prepend(slsPtrArray *self, void *ptr)
{
  sls_ptrarray_insert(self, 0, ptr);


}
