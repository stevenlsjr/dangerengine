//
// Created by Steven on 6/23/15.
//


#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include "test-utils.h"
#include <vector>

using namespace std;

class ArrayTest : public ::testing::Test {
protected:

  vector<int> nums = {0, 1, 2, 3};
  slsArray *a = nullptr;

  virtual void SetUp()
  {

    a = sls_array_new((void *) nums.data(), sizeof(int), sizeof(nums) / sizeof(int));
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
  const size_t valid_idx = 0;
  const size_t invalid_idx = 100;



  auto n = static_cast<int*>(sls_msg(a, get, valid_idx));
  auto overflow = static_cast<int*>(nullptr);

  sls::silence_stderr([&](){
     overflow = static_cast<int*>(sls_msg(a, get, invalid_idx));
  });


  EXPECT_EQ(nullptr, overflow);
  EXPECT_NE(nullptr, n);
  EXPECT_EQ(nums[valid_idx], *n);
}

