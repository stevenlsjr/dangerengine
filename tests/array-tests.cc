//
// Created by Steven on 6/23/15.
//


#include <gtest/gtest.h>
#include "../src/dangerengine.h"

class ArrayTest : public ::testing::Test {
protected:

  slsArray *a = nullptr;

  virtual void SetUp()
  {
    int nums[] = {0, 1, 2, 3};

    a = sls_array_new((void *) nums, sizeof(int), sizeof(nums) / sizeof(int));
  }

  virtual void TearDown()
  {
    if (a) { sls_msg(a, dtor); }
  }
};


TEST_F(ArrayTest, ArratInit) {
  EXPECT_NE(nullptr, a);
}

TEST_F(ArrayTest, GetElem) {
  auto n = static_cast<int*>(sls_msg(a, get, 0));
  auto overflow = static_cast<int*>(sls_msg(a, get, 100));

  EXPECT_EQ(nullptr, overflow);
  ASSERT_NE(nullptr, n);
  EXPECT_EQ(0, *n);
}

