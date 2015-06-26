//
// Created by Steven on 6/23/15.
//


#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include "test-utils.h"
#include <vector>

using namespace std;

typedef int VALUE_TYPE;

class ArrayTest : public ::testing::Test {
protected:

  vector<VALUE_TYPE> nums = {0, 1, 2, 3};
  slsArray *a = nullptr;

  virtual void SetUp()
  {

    a = sls_array_new((void *) nums.data(),
                      sizeof(VALUE_TYPE),
                      nums.size());
  }

  virtual void TearDown()
  {
    if (a) { sls_msg(a, dtor); }
  }
};


TEST_F(ArrayTest, ArratInit) {
  EXPECT_NE(nullptr, a);
  EXPECT_EQ(nums.size(), sls_msg(a, length));
  EXPECT_EQ(sizeof(VALUE_TYPE), sls_msg(a, element_size));
}


TEST_F(ArrayTest, GetElem) {
  const size_t invalid_idx = 100;


  auto overflow = static_cast<VALUE_TYPE*>(nullptr);

  sls::silence_stderr([&](){
     overflow = static_cast<VALUE_TYPE*>(sls_msg(a, get, invalid_idx));
  });


  for (auto i=0lu; i<nums.size(); ++i) {
    auto ptr = static_cast<VALUE_TYPE*>(sls_msg(a, get, i));

    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(nums[i], *ptr);
  }
  EXPECT_EQ(nullptr, overflow);
}

TEST_F(ArrayTest, Copy)
{
  auto cpy = sls_msg(a, copy);
  void const *a_ptr = sls_msg(a, get, 0);
  void const *cpy_ptr = sls_msg(cpy, get, 0);

  size_t size = sls_msg(a, element_size) * sls_msg(a, length);

  EXPECT_NE(cpy, a);
  EXPECT_NE(a_ptr, cpy_ptr);

  EXPECT_EQ(0, memcmp(a_ptr, cpy_ptr, size));
}

TEST_F(ArrayTest, GetMacro)
{
  auto idx = 0;
  for (auto const &i : nums) {
    EXPECT_EQ(i, SLS_ARRAY_IDX(a, VALUE_TYPE, idx));
    ++idx;
  }
}
