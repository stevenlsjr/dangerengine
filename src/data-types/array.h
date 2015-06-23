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
                    void *data,
                    size_t esize,
                    size_t n_elements);

  void (*dtor)(slsArray *self);

  size_t (*length)(slsArray *self);

  size_t (*element_size)(slsArray *self);

  void *(*get)(slsArray const *self, size_t i);

  void (*set)(slsArray *self, size_t i, void *value);



  slsArray_p *priv;
};

slsArray const *sls_array_class();

slsArray *sls_array_new(void *array, size_t element_size, size_t n_elements);


#endif //DANGERENGINE_ARRAY_H
