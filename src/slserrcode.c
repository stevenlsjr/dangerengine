//
// Created by Steven on 7/11/15.
//

/**
 * @file slserrcode.c
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

#include "slserrcode.h"
#include <stdlib.h>
#include "slsutils.h"

#include <pthread.h>
#include <sls-gl.h>

#define SLS_ERRSTACK_MAXSIZE 100

static slsError error_stack[SLS_ERRSTACK_MAXSIZE] = {};
static size_t error_stack_count = 0;
static pthread_mutex_t err_lock = PTHREAD_MUTEX_INITIALIZER;



void sls_push_error(slsError err)
{
  if (error_stack_count > SLS_ERRSTACK_MAXSIZE) {
    sls_log_err("error-code stack overflow!");
    sls_setup_errstack(); // reset error stack
  }

  pthread_mutex_lock(&err_lock);
  // push code to top of stack
  if (SLS_ERRSTACK_MAXSIZE <= error_stack_count) {
    error_stack_count = 0;
  }
  error_stack[error_stack_count] = err;

  ++error_stack_count;
  pthread_mutex_unlock(&err_lock);

}

slsError sls_geterr()
{
  if (error_stack_count < 1) {
    return SLS_OK;
  } else {
    pthread_mutex_lock(&err_lock);
    --error_stack_count;
    slsError res = error_stack[error_stack_count];
    pthread_mutex_unlock(&err_lock);
    return res;
  }
}

char const *sls_strerr(slsError err)
{
  typedef struct err_pair_s {
    slsError key;
    char *value;
  } err_pair_t;

  static const err_pair_t pairs[] = {
      {SLS_OK, "SLS_OK, no error"},
      {SLS_MALLOC_ERR, "SLS_MALLOC_ERR, malloc error"},
      {SLS_INDEX_OVERFLOW, "SLS_INDEX_OVERFLOW, index overflow"}
  };

  static char *unknown = "unknown error";

  char *rval = NULL;
  const size_t len = sizeof(pairs)/sizeof(err_pair_t);
  for (int i=0; i<len; ++i) {
    if (err == pairs[i].key) {
      rval = pairs[i].value;
      break;
    }
  }

  if (!rval) {
    rval = unknown;
  }


  return rval;
}

size_t sls_get_error_count()
{
  return error_stack_count;
}

void sls_setup_errstack()
{
  pthread_mutex_lock(&err_lock);
  error_stack_count = 0;
  pthread_mutex_unlock(&err_lock);
}

void sls_teardown_errstack()
{
  pthread_mutex_lock(&err_lock);
  error_stack_count = 0;
  pthread_mutex_unlock(&err_lock);


}

void sls_push_glerrors(GLenum const *errs, size_t n_errors, char const *fn_name)
{
  char const *_fn_name = fn_name? fn_name: "";

  for (int i=0; i<n_errors; ++i){
    GLenum err = errs[i];
    //sls_log_err("%s glError: %x", _fn_name err);
    sls_push_error((slsError) err);
  }
}

void sls_glerror_unwind()
{
  GLenum err = GL_NO_ERROR;

  GLenum *buffer = NULL;
  size_t buff_size = 32;
  size_t n_errors = 0;
  buffer = calloc(buff_size, sizeof(GLenum));
  sls_checkmem(buffer);

  while((err = glGetError()) != GL_NO_ERROR) {
    if (n_errors >= buff_size) {
      buff_size *= 2;
      buffer = realloc(buffer, buff_size);
    }
    if (n_errors > buff_size) {
      sls_push_glerrors(buffer, n_errors, NULL);
      n_errors = 0;
    }
    buffer[n_errors] = err;

    n_errors++;
  }

  sls_push_glerrors(buffer, n_errors, NULL);



  free(buffer);

  return;
  error:
    exit(EXIT_FAILURE);

}
