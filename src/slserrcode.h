/// @file "slserrcode.h"
/// Created by Steven on 7/11/15.
///

/**
 * @file slserrcode.h
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
 *    and/or other materials provided with the dist.
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

#ifndef DANGERENGINE_SLSERRCODE_H
#define DANGERENGINE_SLSERRCODE_H

#include <err.h>
#include <errno.h>
#include <stdlib.h>

typedef enum slsError {
  SLS_OK = 0,
  SLS_MALLOC_ERR,
  SLS_INDEX_OVERFLOW,
  SLS_OTHER
} slsError;

void sls_setup_errstack();

void sls_teardown_errstack();

/**
 * @brief reports an error by pusing it onto the errorstack.
 */
void sls_push_error(slsError err);

/**
 * @brief Pops the top error from the stack and returns it
 */
slsError sls_geterr();

/**
 * @brief returns the name of an error code as a static string
 * @return a string corresponding to the given error. It is statically
 * allocated and must not be freed by user
 */
char const *sls_strerr(slsError err);

/**
 * @brief returns number of errors on the stack
 */
size_t sls_get_error_count();

/**
 * @brief places glError stack onto application stack, clearing
 * opengl's errors
 */
void sls_glerror_unwind();

#endif //DANGERENGINE_SLSERRCODE_H

