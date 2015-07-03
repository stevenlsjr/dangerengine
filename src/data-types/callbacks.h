///
/// Created by Steven on 7/2/15.
/// @file $FILE$
/// @brief data type callbacks
///

#ifndef DANGERENGINE_CALLBACKS_H
#define DANGERENGINE_CALLBACKS_H


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

struct slsCallbackTable {
  slsFreeFn free_fn;
  slsCopyFn copy_fn;
  slsCmpFn cmp_fn;
};



#endif //DANGERENGINE_CALLBACKS_H
