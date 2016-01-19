//
// Created by Steven on 5/3/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <cppapi.h>

using namespace std;

bool operator==(slsIPoint const &a, slsIPoint const &b)
{
  return bool(sls_ipoint_eq(&a, &b));
}

ostream &operator<<(ostream &out, slsIPoint const &p)
{
  return out << "slsIPoint{" << p.x << ", " << p.y << "}";
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

//---------------------------------modelview stack tests---------------------------------------

#include <kazmath/kazmath.h>

class MatStackTest : public ::testing::Test {

protected:

  slsMatrixStack *mat = nullptr;


  virtual void SetUp()
  {
    mat = new slsMatrixStack;
    mat->matrices = NULL;
    mat = sls_matrix_stack_init(mat, 16);
    ASSERT_TRUE(mat);
    ASSERT_TRUE(mat->matrices);

  }

  virtual void TearDown()
  {
    delete sls_matrix_stack_dtor(mat);
  }
};

TEST_F(MatStackTest, Peek)
{
  EXPECT_EQ(0, mat->n_matrices);
  EXPECT_FALSE(sls_matrix_stack_peek(mat));


  size_t inc = 2;
  mat->n_matrices = inc;
  EXPECT_EQ(mat->matrices + inc - 1, sls_matrix_stack_peek(mat));

}

TEST_F(MatStackTest, Push)
{
  kmMat4 m;
  kmMat4Identity(&m);
  sls_matrix_stack_push(mat, &m);

  EXPECT_TRUE(kmMat4IsIdentity(sls_matrix_stack_peek(mat)));
}

TEST_F(MatStackTest, PushSeveral)
{
  for (int i = 0; i < 10; ++i) {
    kmMat4 m;
    m.mat[0] = float(i);
    sls_matrix_stack_push(mat, &m);

    EXPECT_NEAR(float(i), sls_matrix_stack_peek(mat)->mat[0], 0.001);
  }
}

TEST_F(MatStackTest, DefaultPush)
{
  sls_matrix_glpush(mat);
  EXPECT_TRUE(kmMat4IsIdentity(sls_matrix_stack_peek(mat)));
  kmMat4 m;
  float n = 3.0;
  m.mat[0] = n;

  sls_matrix_stack_push(mat, &m);
  sls_matrix_glpush(mat);

  EXPECT_NEAR(n, sls_matrix_stack_peek(mat)->mat[0], 0.001);
}

TEST_F(MatStackTest, Translate)
{
  sls_matrix_glpush(mat);
  kmVec3 t = {3.0, 0.0, 0.0};
  kmMat4 m;
  kmMat4Identity(&m);
  kmMat4Translation(&m, t.x, t.y, t.z);

  sls_matrix_glidentity(mat);
  sls_matrix_gltranslate(mat, t);
  EXPECT_TRUE(kmMat4AreEqual(&m, sls_matrix_stack_peek(mat)));
}