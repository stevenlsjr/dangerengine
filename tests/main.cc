//
// Created by Steven on 4/27/15.
//

#include <glib-2.0/glib.h>
#include <dangerengine.h>

extern "C" void data_tests_main();


int main(int argc, char **argv)
{
  g_test_init(&argc, &argv, NULL);

  g_test_add_func("/sanity/sanity-test", []() {
    g_assert_true(true);
  });

  data_tests_main();
  g_test_set_nonfatal_assertions();
  return g_test_run();
}
