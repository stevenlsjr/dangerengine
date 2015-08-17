//
// Created by Steven on 7/24/15.
//

#include <dangerengine.h>
#include <data-types/ptrarray.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;

class PtrArrayTests : public ::testing::Test {
protected:
  slsPtrArray *array;
  vector<int> nums;
  vector<int*> num_ptrs;

public:
  PtrArrayTests():
      nums(vector<int>{0, 1, 2, 4, 100}),
      array(new slsPtrArray[1]()){
    for (auto &i: nums) {
      num_ptrs.push_back(&i);
    }

    sls_ptrarray_init(array,
                      reinterpret_cast<void**>(&num_ptrs[0]),
                      num_ptrs.size(),
                      NULL);

  }

  virtual ~PtrArrayTests() {
    delete sls_ptrarray_dtor(array);
  }
};

TEST_F(PtrArrayTests, Construction) {
  EXPECT_TRUE(array);
  EXPECT_EQ(num_ptrs.size(), array->n_elements);
  for (auto i=0; i<array->n_elements; ++i) {

  }
}

TEST_F(PtrArrayTests, Insert)
{
  auto old_size = array->n_elements;
  int foo = 100;
  sls_ptrarray_insert(array, array->n_elements, &foo);

  EXPECT_EQ(old_size + 1, array->n_elements);
  EXPECT_EQ(&foo, static_cast<int *>(array->data[old_size]));
}

TEST_F(PtrArrayTests, Prepend)
{
  int foo = 100;


  sls_ptrarray_prepend(array, &foo);
  EXPECT_EQ(&foo, array->data[0]);
  for (auto i=0; i<num_ptrs.size(); ++i){
    ASSERT_EQ(num_ptrs[i], static_cast<int*>(array->data[i + 1]));
  }
}


TEST_F(PtrArrayTests, Append)
{
  int foo = 100;


  sls_ptrarray_append(array, &foo);
  EXPECT_EQ(&foo, array->data[array->n_elements - 1]);
}

