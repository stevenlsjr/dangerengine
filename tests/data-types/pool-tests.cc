/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/23/16, Steven
 * 
 **/


#include <gtest/gtest.h>
#include <dangerengine.h>


class PoolTests : public ::testing::Test {

protected:
  slsPool *p;


  virtual void SetUp()
  {
    p = sls_pool_create(NULL);
  }

  virtual void TearDown()
  {

    sls_pool_delete(p);
  }


};


TEST_F(PoolTests, Alloc)
{
  auto n = 100;
  int *np = (int *) sls_palloc(p, sizeof(int));
  *np = n;
  EXPECT_EQ(n, *np);
}


TEST_F(PoolTests, BigAlloc)
{
  for (auto j = 0; j < N_POOL_ARENAS * 2; ++j) {
    auto alen = (size_t) 200000;
    auto buff = (int *) sls_palloc(p, sizeof(int) * alen);

    EXPECT_TRUE(buff);
    if (buff) {
      for (auto i = 0; i < alen - 1; ++i) {
        buff[i] = i;
        EXPECT_EQ(buff[i], i);

      }
    } else {
      __volatile int aweae  = 10;
    }

  }

}

