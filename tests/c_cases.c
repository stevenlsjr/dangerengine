//
// Created by Steven on 4/27/15.
//

#include <stdio.h>
#include "c_cases.h"
#include "../src/dangerengine.h"

int tests_run;

void c_test_setup(void *data);

void c_test_teardown(void *data);

/* file: minunit.h */
#define mu_assert(message, test) do {                                 \
    if (!(test)) {                                                    \
      sls_log_err("assertion %s failed: case %s", #test, __func__);   \
      return message;                                                 \
    }                                                                 \
  } while (0)

#define mu_run_test(test, setup, teardown) do {                       \
  if (setup) (setup)(NULL);                                           \
  char const *message = test(); tests_run++;                          \
  if (message){                                                       \
    if (teardown) (teardown)(NULL);                                   \
    return message;                                                   \
  }                                                                   \
} while (0)


void c_test_setup(void *data) { }

void c_test_teardown(void *data) { }


char const *c_test_ctx()
{
  slsContext *ctxA = NULL;

  ctxA = sls_context_new("waegaew", 10, 10);
  mu_assert("cxtA is non-null", ctxA);

  sls_msg(ctxA, dtor);

  return NULL;
}


char const *c_run_all_tests()
{
  MinunitTestFn fns[] = {c_test_ctx};
  const size_t n = sizeof(fns) / sizeof(MinunitTestFn);

  for (size_t i = 0; i < n; ++i) {

    mu_run_test(fns[i], c_test_setup, c_test_teardown);
  }

  return NULL;

}