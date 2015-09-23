///
/// Created by Steven on 7/2/15.
/// @file $FILE$
/// @brief data type callbacks
///


/**
 * @file callbacks.h
 * @brief 
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea. 
**/

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

int sls_cmp_voidptr(void const *a, void const *b);


#endif //DANGERENGINE_CALLBACKS_H
