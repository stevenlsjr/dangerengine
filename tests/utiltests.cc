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



/*-------------------------------------------------
 * preprocessor tests
 *-------------------------------------------------*/

TEST(CPPTests, TokenToString)
{
  auto str0 = SLS_TOK_TO_STR(hello_world);

  EXPECT_STREQ("hello_world", str0) << R"(
SLS_TOK_TO_STR should produce a string from
given token)" << str0;

  auto str1 = SLS_TOK_TO_STR(a
                                 b
                                 c {()});  // note: ignores whitespace
  EXPECT_STREQ("a b c {()}", str1) << R"(
SLS_TOK_TO_STR should produce a string from
given token)" << str1;
}





TEST(CPPTests, TokenConcat)
{

#define sample_exp() \
  int SLS_GENSYM(foo) = 10; \
  EXPECT_EQ(10, SLS_GENSYM(foo)) << "If this compiles, it should work";

#define sample_max(a, b) ({ \
  int SLS_GENSYM(aa_) = (a); \
  int SLS_GENSYM(bb_) = (b); \
  SLS_GENSYM(aa_) > SLS_GENSYM(bb_)? SLS_GENSYM(aa_): SLS_GENSYM(bb_); \
})

  sample_exp(); // note, this macro  exposes variables to the scope
  sample_exp();

  // ensuring the sample_max macro compiles when invoked on the same line
  EXPECT_EQ(10, sample_max(10, 1));
  EXPECT_EQ(10, sample_max(10, sample_max(1, 2)));


#undef sample_exp
#undef sample_max

}

#undef sample_gensym