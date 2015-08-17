///
/// Created by Steven on 7/2/15.
/// @file $FILE$
/// @brief data type callbacks
///

#ifndef DANGERENGINE_CALLBACKS_H
#define DANGERENGINE_CALLBACKS_H

#include <inttypes.h>


typedef struct slsCallbackTable slsCallbackTable;


/**
 * @brief function pointer type for copying btree data
 * @details a function pointer taking a const void pointer and
 *
 * @param data pointer to data for copying
 * @return copy of input data
 */
typedef void *(*slsCopyFn)(void const *data);

/**
 * @brief function pointer type for freeing btree data
 * @details takes a void pointer, to free associated memory
 * and close files
 *
 * @param pointer to data [description]
 * @return [description]
 */
typedef void (*slsFreeFn)(void *data);


/**
 * @brief function pointer type for comparing two orderable values
 *
 * @return returns < 0 if lhs has a lower order,
 * > 0 if rhs has lower order, and 0 if they are equal
 */
typedef int (*slsCmpFn)(void const *lhs, void const *rhs);

typedef uint32_t (*slsHashFn)(void const *key);

struct slsCallbackTable {
  slsFreeFn free_fn;
  slsCopyFn copy_fn;
  slsCmpFn cmp_fn;
};

/*---------------------------------
 * sample callback functions
 *---------------------------------*/

/**
 * @brief does not copy pointer,
 * instead casting it as non-const and returning
 * the value.
 * @detail used when you wish to store a raw pointer without
 * managing its memory within the structure
 */
void *sls_copy_assign(void const *ptr);

/**
 * @brief copies a c-string passed as a void pointer
 */
void *sls_copy_string(void const *str);

/**
 * @brief compares two c-strings passed as a void pointer
 */
int sls_cmp_string(void const *a, void const *b);

/**
 * @brief compares two pointers to integers passed as a void pointer
 */
int sls_cmp_intptr(void const *a, void const *b);

/**
 * @brief compares two pointers to unsigned integers passed as a void pointer
 */
int sls_cmp_uintptr(void const *a, void const *b);


#endif //DANGERENGINE_CALLBACKS_H
