//
// Created by Steven on 4/27/15.
//

#include <dangerengine.h>
#include <unity.h>


int main(int argc, char **argv)
{
  int res = EXIT_FAILURE;
  if (TEST_PROTECT()) {
    extern int data_tests_main(void);
    extern int math_tests_main(void);
    res = data_tests_main();
    res = math_tests_main() && res;
  }
  return res;
}
