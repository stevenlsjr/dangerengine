//
// Created by Steven on 4/27/15.
//

#include <stdio.h>
#include "c_cases.h"
#include "../src/dangerengine.h"

/* file: minunit.h */
#define mu_assert(message, test) do { \
        if (!(test)) { \
            sls_log_err("assertion %s failed: case %s", #test, __func__);   \
            return message;\
        }\
    } while (0)

#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

static int tests_run;


char *c_test_ctx()
{
    mu_assert("1 == 1", 1 == 2);
    return NULL;
}

char *c_run_all_tests()
{
    MinunitTestFn fns[] = {c_test_ctx};
    const size_t n = sizeof(fns) / sizeof(MinunitTestFn);
    for (size_t i = 0; i < n; ++i) {
        mu_run_test(fns[i]);
    }
    return NULL;
}