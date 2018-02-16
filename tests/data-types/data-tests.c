//
// Created by steve on 6/6/17.
//


#include <dangerengine.h>
#include <unity.h>



typedef struct {
  slsArray *array;
} DataFix;

static void setup(DataFix *fix, void const *data)
{
  fix->array = NULL;
  fix->array = sls_array_init((slsArray *) malloc(sizeof(slsArray)), (int[]) {}, sizeof(int), 0);
}

static void teardown(DataFix *fix, void const *data)
{
  free(sls_array_dtor(fix->array));
}



static void test_array_init()
{
  DataFix _fix;
  DataFix *fix = &_fix;
  setup(fix, NULL);



  size_t len = sls_array_length(fix->array);
  int i = 0xcafebabe;
  sls_array_append(fix->array, &i);
  TEST_ASSERT_EQUAL(len + 1, sls_array_length(fix->array));

  // test array mutation
  SLS_ARRAY_IDX(fix->array, int, 0) = 10;
  TEST_ASSERT_EQUAL(10, SLS_ARRAY_IDX(fix->array, int, 0));

  teardown(fix, NULL);

}

static void test_array_insert_many()
{
  DataFix _fix;
  DataFix *fix = &_fix;
  setup(fix, NULL);


  size_t len = sls_array_length(fix->array);
  int arr[] = {0, 1, 2, 3};
  sls_array_insert_array(fix->array, 0, arr, 4);

  TEST_ASSERT_EQUAL(len + 4, sls_array_length(fix->array));

  teardown(fix, NULL);

}

static void test_array_remove()
{

  DataFix _fix;
  DataFix *fix = &_fix;
  setup(fix, NULL);


  size_t len = sls_array_length(fix->array);
  int first_item = 50;
  int other_items = 100;
  slsArray *a = fix->array;
  sls_array_append(a, &first_item);
  sls_array_append(a, &other_items);
  sls_array_append(a, &other_items);

  size_t size = sls_array_length(fix->array);
  TEST_ASSERT_EQUAL(size, len + 3);

  sls_array_append(a, &other_items);

  int expected_items[] = {50, 100, 100, 100};
  const int *arr = sls_array_cget(fix->array, 0);
  TEST_ASSERT_EQUAL_INT_ARRAY(expected_items, arr, 4);

  sls_array_remove(fix->array, sls_array_length(fix->array) - 1); // try removing item at end of array
  sls_array_remove(fix->array, 0); // try removing item at start of array

  int expected2[] = {100, 100};
  TEST_ASSERT_EQUAL_INT_ARRAY(expected2, (const int*)sls_array_cget(fix->array, 0), 2);

  teardown(fix, NULL);

}

static void test_array_foreach()
{
  DataFix _fix;
  DataFix *fix = &_fix;
  setup(fix, NULL);


  int items[] = {0, 1, 2, 3, 4, 5};
  const size_t len = sizeof(items) / sizeof(*items);
  for (int i = 0; i < len; ++i) {

    sls_array_append(fix->array, items + i);

  }
  slsArrayItor itor_;
  slsArrayItor *itor = &itor_;
  SLS_ARRAY_FOREACH(fix->array, itor) {

    int *elt = itor->elt;
  }

  teardown(fix, NULL);

}


int data_tests_main()
{
  UNITY_BEGIN();

  RUN_TEST(test_array_init);
  RUN_TEST(test_array_insert_many);
  RUN_TEST(test_array_remove);
  RUN_TEST(test_array_foreach);

  return UNITY_END();

}