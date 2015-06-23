//
// Created by Steven on 5/3/15.
//

#include <stddef.h>

#ifndef DANGERENGINE_ARRAY_H
#define DANGERENGINE_ARRAY_H

typedef struct slsArray slsArray;
typedef struct slsArray_p slsArray_p;

struct slsArray {

  /**
   * @brief initializer
   *
   * @param data array of elements.
   */
  slsArray *(*init)(slsArray *self,
                    void const *data,
                    size_t esize,
                    size_t n_elements);

  void (*dtor)(slsArray *self);

  size_t (*length)(slsArray const *self);

  size_t (*element_size)(slsArray const *self);

  void *(*get)(slsArray const *self, size_t i);

  void (*set)(slsArray *self, size_t i, void *value);

  slsArray *(*copy)(slsArray const *self);

  slsArray_p *priv;
};

slsArray const *sls_array_class();

slsArray *sls_array_new(void const *array,
                        size_t element_size,
                        size_t n_elements);


/**
 * @brief convenience method for retrieving index values.
 * @detail WARNING: dereferences pointer given by slsArray::get
 * will dereference a NULL value.
 */
#define SLS_ARRAY_IDX(array, T, idx)  \
 (*((T*)(sls_msg((array),              \
                  get,                \
                  (idx)))))

#endif //DANGERENGINE_ARRAY_H
