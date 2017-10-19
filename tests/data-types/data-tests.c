//
// Created by steve on 6/6/17.
//


#include <dangerengine.h>
#include <glib.h>
#include "../testing.h"

typedef struct {
  slsArray *array;
} DataFix;

static void setup(DataFix *fix, void const *data)
{
  fix->array = NULL;
  fix->array = sls_array_init((slsArray *) malloc(sizeof(slsArray)), (int[]) {}, sizeof(int), 0);
  g_assert_true(fix->array);
}

static void teardown(DataFix *fix, void const *data)
{
  free(sls_array_dtor(fix->array));
}

static void abrt_handler(int s)
{
  g_test_fail();
  exit(EXIT_FAILURE);
  signal(SIGABRT, abrt_handler);
}


static void test_array_init(DataFix *fix, void const *data)
{
  (void) data;
  g_assert_true(fix->array);
  size_t len = sls_array_length(fix->array);
  g_assert_cmpint(len, ==, 0);
  int i = 0xcafebabe;
  sls_array_append(fix->array, &i);

  signal(SIGABRT, abrt_handler);
  g_assert_cmphex(SLS_ARRAY_IDX(fix->array,
                      int, 0), ==, i);
  // test array mutation
  SLS_ARRAY_IDX(fix->array, int, 0) = 10;
  g_assert_cmphex(SLS_ARRAY_IDX(fix->array,
                      int, 0), ==, 10);

}

static void test_array_insert_many(DataFix *fix, void const *data)
{
  int arr[] = {0, 1, 2, 3};
  sls_array_insert_array(fix->array, 0, arr, 4);

  g_assert_cmpint(SLS_ARRAY_IDX(fix->array,
                      int, 0), ==, 0);
  g_assert_cmpint(SLS_ARRAY_IDX(fix->array,
                      int, 1), ==, 1);
  g_assert_cmpint(SLS_ARRAY_IDX(fix->array,
                      int, 2), ==, 2);
  g_assert_cmpint(SLS_ARRAY_IDX(fix->array,
                      int, 3), ==, 3);
}

static void test_array_remove(DataFix *fix, void const *data)
{
  int first_item = 50;
  int other_items = 100;
  slsArray *a = fix->array;
  sls_array_append(a, &first_item);
  sls_array_append(a, &other_items);
  sls_array_append(a, &other_items);

  size_t size = sls_array_length(fix->array);
  sls_array_append(a, &other_items);
  g_assert_cmpint(size + 1, ==, sls_array_length(fix->array));

  sls_array_remove(fix->array, size); // try removing item at end of array
  g_assert_cmpint(size, ==, sls_array_length(fix->array));

  g_assert_cmpint(first_item, ==, SLS_ARRAY_IDX(fix->array,
      int, 0));
  sls_array_remove(fix->array, 0); // try removing item at start of array
  g_assert_cmpint(size - 1, ==, sls_array_length(fix->array));
  g_assert_cmpint(other_items, ==, SLS_ARRAY_IDX(fix->array,
      int, 0)); // first element should now be 100


}

static void test_array_foreach(DataFix *fix, void const *data)
{
  int items[] = {0, 1, 2, 3, 4, 5};
  const size_t len = sizeof(items) / sizeof(*items);
  for (int i = 0; i < len; ++i) {

    sls_array_append(fix->array, items + i);

  }
  slsArrayItor itor_;
  slsArrayItor *itor = &itor_;
  SLS_ARRAY_FOREACH(fix->array, itor) {

    int *elt = itor->elt;
    g_assert_cmpint(items[itor->index], ==, *elt);
  }
}


void data_tests_main()
{

  struct TestTuple {
    char const *path;

    void (*fn)(DataFix *, void const *);
  };

  struct TestTuple tests[] = {
      {"/data/array/init",        test_array_init},
      {"/data/array/insert-many", test_array_insert_many},
      {"/data/array/remove",      test_array_remove},
      {"/data/array/foreach",     test_array_foreach}

  };
  size_t const tests_len = sizeof(tests) / sizeof(*tests);
  for (int i = 0; i < (int) tests_len; ++i) {
    g_test_add(tests[i].path, DataFix, NULL, setup, tests[i].fn, teardown);
  }

}