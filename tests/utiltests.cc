//
// Created by Steven on 7/11/15.
//

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <thread>
#include "../src/dangerengine.h"
#include <random>
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace testing;

class ErrorTests : public ::testing::Test {
public:
  ErrorTests()
  {
    while(sls_get_error_count() > 0) {
      sls_geterr();
    }
  }

protected:

};

TEST_F(ErrorTests, MallocError)
{
  sls_push_error(SLS_MALLOC_ERR);
  EXPECT_EQ(SLS_MALLOC_ERR, sls_geterr());
  EXPECT_EQ(SLS_OK, sls_geterr());

  EXPECT_EQ(0, sls_get_error_count());
}

TEST_F(ErrorTests, CheckMacro)
{
  auto const code = 100;
  sls_check_code(0 == 1, code, "error should be sent");
  {
    FAIL() << "error macro should skip this line";
  } //
error:
  EXPECT_EQ(code, sls_geterr());
}


void *thread_safety_run(void *data)
{

  auto dist = uniform_int_distribution<int>(0, 100000);
  random_device rd;

  // sleep between 0 and .1 seconds
  auto mu_sec = useconds_t(dist(rd));
  usleep(mu_sec);

  // push given error to error stack
  slsError *err = (slsError*)data;
  sls_push_error(*err);

  return NULL;
}

TEST_F(ErrorTests, ThreadSafety)
{
  auto expect_errcount = 10lu;

  auto i = SLS_INDEX_OVERFLOW;
  auto m = SLS_MALLOC_ERR;

  auto threads = vector<pair<pthread_t, pthread_t>>(expect_errcount);
  auto merror = 0, ierror = 0;


  for (auto &p: threads) {
    int rc;
    rc = pthread_create(&p.first, NULL, thread_safety_run, &i);
    ASSERT_EQ(0, rc);
    rc = pthread_create(&p.second, NULL, thread_safety_run, &m);
    ASSERT_EQ(0, rc);

  }

  for (auto &p: threads) {
    pthread_join(p.first, NULL);
    pthread_join(p.second, NULL);
  }


  slsError err;
  while ((err = sls_geterr()) != SLS_OK) {
    if (err == SLS_INDEX_OVERFLOW) {
      ++ierror;
    } else if (err == SLS_MALLOC_ERR) {
      ++merror;
    }
  }

  EXPECT_EQ(expect_errcount, ierror);
  EXPECT_EQ(expect_errcount, merror);

}