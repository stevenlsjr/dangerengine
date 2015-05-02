//
// Created by Steven on 4/27/15.
//

#include <gtest/gtest.h>
#include "c_cases.h"

using namespace std;





int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

#ifdef WIN32
    // prevents visual studio from exiting early
    auto res = RUN_ALL_TESTS();
    getchar();
    return res;

#else
    return RUN_ALL_TESTS();
#endif

}
