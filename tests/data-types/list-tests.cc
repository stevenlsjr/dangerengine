/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/11/15, Steven
 * 
 **/

#include <gtest/gtest.h>
#include <dangerengine.h>
#include "../test-utils.h"

#include <algorithm>
#include <vector>

using namespace std;

class ListTests : public ::testing::Test {
protected:
  slsLinkedList *ls;
  vector<int> nums;

  void setup_list()
  {
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
                nums(vector<int>{0, 1, 2, 3, 4})
  {
    setup_list();
  }

  virtual ~ListTests()
  {
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
    int *ptr = static_cast<int *>(itor->data);
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
    int n = *static_cast<int *>(itor->data);
    EXPECT_EQ(nums[idx], n);
    ++idx;
  }
}


