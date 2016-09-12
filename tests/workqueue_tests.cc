/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 9/12/16, Steven
 * 
 **/
#include <gtest/gtest.h>
#include <dangerengine.h>

#include <vector>

using namespace std;


class WorkQueueTest : public ::testing::Test {
public:

  slsWorkScheduler *threadpool;

  slsTaskQueue *tasks() { return &threadpool->waiting; };

protected:

  virtual void SetUp()
  {
    threadpool = sls_workscheduler_init(new slsWorkScheduler());
  }

  virtual void TearDown()
  {
    if (threadpool) {
      delete (sls_workscheduler_dtor(threadpool));
    }
  }
};

TEST_F(WorkQueueTest, EnqueueFifo)
{
  auto nums = vector<int>{1, 2, 3, 4, 5};
  for (auto &i: nums) {
    auto t = sls_make_task(nullptr, &i);
    ASSERT_TRUE(t);
    sls_task_enqueue(tasks(), t);
  }

  auto itor = tasks()->head;
  // ensure first in first out order
  for (auto const &i: nums) {
    if (!itor) {
      ASSERT_TRUE(0) << R"(
number of queue elements and vector
elements mst be equal
)";
      break;
    }
    auto task_i = static_cast<int *>(itor->pdata);
    EXPECT_TRUE(task_i) << "task's parameter data is NULL";
    EXPECT_EQ(i, *task_i);

    itor = itor->next;
  }
}

TEST_F(WorkQueueTest, Dequeue)
{
  auto nums = vector<int>{1, 2, 3, 4, 5};
  for (auto &i: nums) {
    auto t = sls_make_task(nullptr, &i);
    ASSERT_TRUE(t);
    sls_task_enqueue(tasks(), t);
  }

  // ensure dequeue operation uses correct order
  for (auto const &i: nums) {

    auto task = sls_task_dequeue(tasks());
    {
      EXPECT_TRUE(task);
      auto task_i = static_cast<int *>(task->pdata);
      EXPECT_TRUE(task_i);

      EXPECT_EQ(i, *task_i);
    }
    // free loose task memory
    free(task);
  }
}
