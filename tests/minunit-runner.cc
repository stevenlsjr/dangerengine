//
// Created by Steven on 5/2/15.
//

#include <gtest/gtest.h>
#include "c_cases.h"

TEST(MINUNIT, RunCTests)
{
auto const minunit_res = c_run_all_tests();

EXPECT_EQ(0, minunit_res) << "Test failed with message: " << minunit_res << "\n";

RecordProperty("Minunit tests run", tests_run);
}