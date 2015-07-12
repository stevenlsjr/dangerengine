/**
 * // Created by Steven on 5/3/15.
 * @file data-types/array.h
 * @brief __
 */

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

  slsArray_p *priv;
};

slsArray const *sls_array_class();

slsArray *sls_array_new(void const *array,
                        size_t element_size,
                        size_t n_elements);


size_t sls_array_length(slsArray const *self);

size_t sls_array_element_size(slsArray const *self);

void const *sls_array_get(slsArray const *self, size_t i);

/**
 * @brief sets the value of a current element
 * @details [long description]
 *
 * @param t [description]
 * @param i [description]
 * @param value [description]
 * @return [description]
 */
void sls_array_set(slsArray *self, size_t i, void *value);

/**
 * @brief inserts an element, pushing back elements ahead in the array.
 * @details [long description]
 *
 * @param t [description]
 * @param i [description]
 * @param value [description]
 * @return [description]
 */
void sls_array_insert(slsArray *self, size_t i, void *value);

slsArray *sls_array_copy(slsArray const *self);

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
