//
// Created by Steven on 4/27/15.
//

#ifndef DANGERENGINE_C_CASES_H
#define DANGERENGINE_C_CASES_H

#ifdef __cplusplus
extern "C" {
#endif

    
  struct MinunitCaseArray {
    char **cases;
    size_t n;
  };

  typedef char const *(*MinunitTestFn)();
  char const *c_run_all_tests();

  extern int tests_run;

#ifdef __cplusplus
};
#endif

#endif //DANGERENGINE_C_CASES_H

