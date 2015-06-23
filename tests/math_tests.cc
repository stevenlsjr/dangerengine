//
// Created by Steven on 5/3/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"

using namespace std;

bool operator==(slsIPoint const &a, slsIPoint const &b)
{
  return bool(sls_ipoint_eq(&a, &b));
}

ostream& operator<<(ostream &out, slsIPoint const &p)
{
  return out << "slsIPoint{" << p.x <<", "<< p.y << "}";
}

class IPointTests : public ::testing::Test {
protected:
  slsIPoint id = {1, 1};
  slsIPoint zero = {0, 0};
  slsIPoint a = {2, 5};
  slsIPoint b = {10, -11};
};

TEST_F(IPointTests, Add)
{
  slsIPoint exp = {12, -6};
  EXPECT_EQ(exp, sls_ipoint_add(&a, &b));

  // adding zero should be a
  EXPECT_EQ(a, sls_ipoint_add(&zero, &a));
}

TEST_F(IPointTests, Mul)
{
  slsIPoint exp = {20, -55};
  EXPECT_EQ(exp, sls_ipoint_mul(&a, &b));

  // a * id == a
  EXPECT_EQ(a, sls_ipoint_mul(&id, &a));

  // a * zero == zero
  EXPECT_EQ(zero, sls_ipoint_mul(&zero, &a));
}

TEST_F(IPointTests, IDiv)
{
  // a / id == a
  EXPECT_EQ(a, sls_ipoint_idiv(&a, &id));

  // id / a == a fraction rounded to zero
  slsIPoint div = {100, 100};
  EXPECT_EQ(zero, sls_ipoint_idiv(&id, &div));

  // zero / a == zero
  EXPECT_EQ(zero, sls_ipoint_idiv(&zero, &a));
}