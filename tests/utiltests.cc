//
// Created by Steven on 7/11/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"

using namespace std;
using namespace testing;

class UtilTests : public ::testing::Test {
public:
  UtilTests()
  {
    while(sls_get_error_count() > 0) {
      sls_geterr();
    }
  }

protected:

};

TEST_F(UtilTests, MallocError)
{
  sls_push_error(SLS_MALLOC_ERR);
  EXPECT_EQ(SLS_MALLOC_ERR, sls_geterr());
  EXPECT_EQ(SLS_OK, sls_geterr());

  EXPECT_EQ(0, sls_get_error_count());
}

TEST_F(UtilTests, CheckMacro)
{
  auto const code = 100;
  sls_check_code(0 == 1, code, "error should be sent");
  {
    FAIL() << "error macro should skip this line";
  } //
error:
  EXPECT_EQ(code, sls_geterr());


}
