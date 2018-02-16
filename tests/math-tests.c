//
// Created by steve on 9/30/17.
//

#include <unity.h>
#include <dangerengine.h>

#define TEST_ASSERT_VEC_EQUAL(v1, v2, n)

static slsVec2 v2a = {.x = 1.0, .y=2.0};
static slsVec2 v2b = {.x = 1.0, .y=4.4};
static slsVec2 v2c = {.x = 1.0, .y=-4.1f};



static void test_v2_add()
{
  slsVec2 res;
  sls_vec2_add(&res, &v2a, &v2b);
  TEST_ASSERT_EQUAL_FLOAT(2.0, res.x);
  TEST_ASSERT_EQUAL_FLOAT(6.4, res.y);
}

int math_tests_main()
{
  UNITY_BEGIN();

  RUN_TEST(test_v2_add);
  return UNITY_END();
}