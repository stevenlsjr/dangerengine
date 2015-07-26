//
// Created by Steven on 6/23/15.
//


#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include "test-utils.h"

#include <algorithm>
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
  EXPECT_EQ(nums.size(), sls_array_length(a));
  EXPECT_EQ(sizeof(VALUE_TYPE), sls_array_element_size(a));
}


TEST_F(ArrayTest, GetElem) {
  const size_t invalid_idx = 100;


  VALUE_TYPE const* overflow;

  sls::silence_stderr([&](){
     overflow = static_cast<VALUE_TYPE const*>(sls_array_get(a, invalid_idx));
  });


  for (auto i=0lu; i<nums.size(); ++i) {
    auto ptr = static_cast<VALUE_TYPE const*>(sls_array_get(a, i));

    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(nums[i], *ptr);
  }
  EXPECT_EQ(nullptr, overflow);
}

TEST_F(ArrayTest, Copy)
{
  auto cpy = sls_array_copy(a);
  void const *a_ptr = sls_array_get(a, 0);
  void const *cpy_ptr = sls_array_get(cpy, 0);

  size_t size = sls_array_element_size(a) * sls_array_length(a);

  EXPECT_NE(cpy, a);
  EXPECT_NE(a_ptr, cpy_ptr);

  EXPECT_EQ(0, memcmp(a_ptr, cpy_ptr, size));
}

TEST_F(ArrayTest, GetMacro)
{
  auto len = sls_array_length(a);
  for (auto i=0; i<len; ++i) {
    auto item = nums[i];
    EXPECT_EQ(item, SLS_ARRAY_IDX(a, VALUE_TYPE, i));
  }
}


TEST_F(ArrayTest, Set)
{
  auto n = VALUE_TYPE(1000);
  auto len = sls_array_length(a);
  for (auto i=0; i<len; ++i) {
    sls_array_set(a, i, &n);
    EXPECT_EQ(n, SLS_ARRAY_IDX(a, VALUE_TYPE, i))  << "index " << i;
  }

}

TEST_F(ArrayTest, Insert)
{
  auto val = VALUE_TYPE(-1);
  nums.emplace(nums.begin(), val);
  sls_array_insert(a, 0, &val);
  auto len = sls_array_length(a);
  EXPECT_EQ(nums.size(), len);

  for (auto i=0; i<len; ++i) {
    EXPECT_EQ(nums[i], SLS_ARRAY_IDX(a, VALUE_TYPE, i))  << "index " << i;
  }
  EXPECT_LE(sls_array_length(a) * sls_array_element_size(a), sls_array_alloc_size(a));

}

TEST_F(ArrayTest, ResizeInsert)
{
  auto n = 1000;
  auto val = VALUE_TYPE(-1);
  for (auto i=0; i<n; ++i) {
    auto idx = 0;
    sls_array_insert(a, idx, &val);
    EXPECT_EQ(val, SLS_ARRAY_IDX(a, VALUE_TYPE, idx));
  }

  EXPECT_LE(sls_array_length(a) * sls_array_element_size(a), sls_array_alloc_size(a));

}

class ListTests : public ::testing::Test {

protected:
  slsLinkedList *ls;
  vector<int> nums;

  void setup_list(){
    if (!ls) {
      return;
    }

    slsListNode *node = nullptr;
    for (auto &i: nums) {
      slsListNode *next = sls_list_node_new(&i,
                                            nullptr,
                                            nullptr,
                                            &ls->callbacks);
      if (!ls->head) {
        ls->head = next;
      }
      sls_list_node_insert_ahead(node, next);
      node = next;
    }
  }

public:

  ListTests() : Test(),
                ls(sls_linked_list_new(NULL)),
                nums(vector<int>{0, 1, 2, 3, 4}) {
    setup_list();
  }

  virtual ~ListTests() {
    if (ls) {
      free(sls_linked_list_dtor(ls));
    }
  }

};

TEST_F(ListTests, ListConstruction)
{
  ASSERT_TRUE(ls);
  ASSERT_TRUE(ls->head);

  auto expect_length = nums.size();
  auto idx = 0lu;

  for (auto itor = ls->head; itor != nullptr; itor = itor->next) {
    int *ptr = static_cast<int*>(itor->data);
    EXPECT_TRUE(ptr);

    if (ptr) {
      EXPECT_EQ(nums[idx], *ptr);
    }


    ++idx;
  }

  // linked list should have same length as nums vector
  EXPECT_EQ(expect_length, idx);

}

/**
 * @brief tests the foreach macro for linked list iteration
 */
TEST_F(ListTests, ListIteration)
{
  auto idx = 0lu;
  slsListNode *itor = NULL;
  SLS_LINKEDLIST_FOREACH(ls->head, itor) {
    int n = *static_cast<int*>(itor->data);
    EXPECT_EQ(nums[idx], n);
    ++idx;
  }
}

