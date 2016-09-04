//
// Created by Steven on 5/3/15.
//

#include <time.h>
#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <sysexits.h>
#include <kazmath/kazmath.h>
#include <iostream>
#include <random>
#include <smmintrin.h>


#include <thread>
#include <chrono>

using namespace std::chrono;
using namespace std;
using namespace testing;

template<typename INFO_T, typename T_TIME_PT>
static inline
decltype(auto) benchmark(INFO_T test_info, string desc, T_TIME_PT start, T_TIME_PT stop)
{
  auto delta = duration_cast<nanoseconds>(stop - start);

  cout << string(test_info->name())
  << "\n\t" << desc << ": performed in " << long(delta.count()) << "µs\n";

  return delta;

}

/// operator overloads to aid test suite
bool operator==(slsIPoint const &a, slsIPoint const &b)
{
  return bool(sls_ipoint_eq(&a, &b));
}

bool operator==(kmMat4 const &a, kmMat4 const &b)
{
  return kmMat4AreEqual(&a, &b);
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



static inline
kmVec4 *sls_mat4_column(kmVec4 *out, kmMat4 const *in, size_t i)
{
  assert(out && in && "arguments must be nonnull");
  assert(i < 4);

  float const *m = in->mat;
  *out = {
      .x = m[i],
      .y = m[4 + i],
      .z = m[8 + i],
      .w = m[12 + i]
  };

  return out;
}

#include <immintrin.h>
#include <gmpxx.h>


static inline
kmMat4 *sls_mat4simd_mul(kmMat4 *out, kmMat4 const *lhs, kmMat4 const *rhs)
{
  float const *a = lhs->mat;
  float const *b = rhs->mat;
  alignas(16) float o[16] = {};
  __m128 row0 = _mm_load_ps(b),
      row1 = _mm_load_ps(b + 4),
      row2 = _mm_load_ps(b + 8),
      row3 = _mm_load_ps(b + 12);


// requires compiler optimization (presumably loop unrolling
// to outperform kmMat4Multiply
  for (
      int i = 0;
      i < 4; ++i) {
    __m128 brod0 = _mm_set1_ps(a[4 * i + 0]),
        brod1 = _mm_set1_ps(a[4 * i + 1]),
        brod2 = _mm_set1_ps(a[4 * i + 2]),
        brod3 = _mm_set1_ps(a[4 * i + 3]);

    __m128 outrow = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(brod0, row0),
                   _mm_mul_ps(brod1, row1)),
        _mm_add_ps(_mm_mul_ps(brod2, row2),
                   _mm_mul_ps(brod3, row3)));

    _mm_store_ps(o + 4 * i, outrow);
  }

  memcpy(out->mat, o, 16 * sizeof(float));

  return
      out;
}


class SimdTests : public Test {
protected:
  chrono::high_resolution_clock hpc;
  default_random_engine rng;
  uniform_real_distribution<float> dist;

  slsVec4Array va;
  slsVec4Array vb;


  virtual void SetUp() override
  {
    dist = uniform_real_distribution<float>(0.0, 20.0);

    for (auto i = 0; i < 4; ++i) {
      va.arr[i] = {dist(rng), dist(rng), dist(rng), 1.0};
      vb.arr[i] = {dist(rng), dist(rng), dist(rng), 0.0};
    }
  }

  virtual void TearDown() override
  {
  }

};

TEST_F(SimdTests, Mat4Mul)
{
  kmMat4 a = {1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 2.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f};

  kmMat4 b = {1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 2.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f};

  kmMat4 expect = {1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 4.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 4.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};


  kmMat4 out, out2;


  EXPECT_EQ(expect, *kmMat4Multiply(&out, &a, &b)) << "sanity test";

  EXPECT_TRUE(bool(sls_mat4simd_mul(&out2, &a, &b)));

  EXPECT_EQ(out, out2);
}



#if 0
/**
 * NOTE: This test will almost always fail without
 * compiler optimizations.
 */
TEST_F(SimdTests, Performance)
{
  kmMat4 a = {1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 2.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f};

  kmMat4 b = {1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 2.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f};


  kmMat4 out, out2;


  auto n_tests = 10;
  auto successes = 0;

  for (auto i = 0; i < n_tests; ++i) {
    auto scalar_start = hpc.now();
    kmMat4Multiply(&out, &a, &b);
    auto scalar_end = hpc.now();

    auto simd_start = hpc.now();
    sls_mat4simd_mul(&out2, &a, &b);
    auto simd_end = hpc.now();


    auto simd_dif = (simd_end - simd_start).count();
    auto scalar_dif = (scalar_end - scalar_start).count();

    //EXPECT_LT(simd_dif, scalar_dif) << "in run " << i << " out of " << n_tests;
    if (simd_dif < scalar_dif) {
      successes++;
    }

  }


}

TEST_F(SimdTests, sls4Vec4Simd)
{

  auto res = sls_simdvec_from_vec4s_val(&va);
  alignas(16) float row[4];

  _mm_store_ps(row, res.xv);
  EXPECT_FLOAT_EQ(row[0], va.arr[0].x);
  EXPECT_FLOAT_EQ(row[0], va.arr[1].x);
  EXPECT_FLOAT_EQ(row[0], va.arr[2].x);
  EXPECT_FLOAT_EQ(row[0], va.arr[3].x);

  _mm_store_ps(row, res.yv);
  EXPECT_FLOAT_EQ(row[1], va.arr[0].y);
  EXPECT_FLOAT_EQ(row[1], va.arr[1].y);
  EXPECT_FLOAT_EQ(row[1], va.arr[2].y);
  EXPECT_FLOAT_EQ(row[1], va.arr[3].y);

  _mm_store_ps(row, res.zv);
  EXPECT_FLOAT_EQ(row[2], va.arr[0].z);
  EXPECT_FLOAT_EQ(row[2], va.arr[1].z);
  EXPECT_FLOAT_EQ(row[2], va.arr[2].z);
  EXPECT_FLOAT_EQ(row[2], va.arr[3].z);

  _mm_store_ps(row, res.wv);

  EXPECT_FLOAT_EQ(row[3], va.arr[0].w);
  EXPECT_FLOAT_EQ(row[3], va.arr[1].w);
  EXPECT_FLOAT_EQ(row[3], va.arr[2].w);
  EXPECT_FLOAT_EQ(row[3], va.arr[3].w);


}
#endif
