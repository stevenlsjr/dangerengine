//
// Created by Steven on 4/27/15.
//

#include <gtest/gtest.h>
#include "c_cases.h"

TEST(MINUNIT, RunCTests)
{
    char *c_test = c_run_all_tests();
    EXPECT_EQ(nullptr, c_test) << "Minunit test failed: " << c_test << "\n";
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    


#ifdef WIN32
    auto res = RUN_ALL_TESTS();
    getchar();
    return res;

#else
    return RUN_ALL_TESTS();
#endif

}
