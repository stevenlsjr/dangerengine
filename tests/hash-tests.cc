//
// Created by Steven on 7/22/15.
//

#include <gtest/gtest.h>
#include <dangerengine.h>


using namespace std;


class TableTests : public ::testing::Test {
protected:

  slsHashTable *table;
  size_t array_size = 100;

  int sample_int = 1000;


  virtual void SetUp()
  {

    slsCallbackTable key_cback = {}, val_cback = {};

    key_cback.cmp_fn = sls_cmp_string;
    val_cback.cmp_fn = sls_cmp_intptr;

    table = sls_hashtable_init(new slsHashTable, array_size, NULL, &key_cback, &val_cback);

    ASSERT_TRUE(table);

  }
  virtual void TearDown()
  {
    delete (sls_hashtable_dtor(table));
  }

};


TEST_F(TableTests, Construction)
{
  EXPECT_TRUE(table->key_callbacks.cmp_fn && table->val_callbacks.cmp_fn);
  EXPECT_TRUE(table->keys && table->vals);
  EXPECT_TRUE(table->hash);
}

TEST_F(TableTests, Insert)
{
  auto const keya = "hello";
  auto const keyb = "say";
  sls_hashtable_insert(table, (void*)keya, SLS_STRING_LENGTH, &sample_int);
  sls_hashtable_insert(table, (void*)keyb, SLS_STRING_LENGTH, &sample_int);

  bool found = false;
  for (auto i=0; i<table->array_size; ++i) {
    auto const ikey = (char const*)table->keys[i];
    if (ikey && strcmp(ikey, keya) == 0) {
      found = true;
    }

  }

  EXPECT_TRUE(found);

  found = false;

  for (auto i=0; i<table->array_size; ++i) {
    auto const ival = (char const*)table->vals[i];
    if (ival && *((int*)ival) == sample_int) {
      found = true;
    }
  }
  EXPECT_TRUE(found);

  found = false;
  for (auto i=0; i<table->array_size; ++i) {
    auto const ikey = (char const*)table->keys[i];
    if (ikey && strcmp(ikey, keyb) == 0) {
      found = true;
    }

  }

  EXPECT_TRUE(found);


}

TEST_F(TableTests, FindInvalid)
{
  auto const key = "hello";
  EXPECT_FALSE(sls_hashtable_find(table, key, SLS_STRING_LENGTH));
}



TEST_F(TableTests, FindValid)
{
  auto const key = "hello";

  sls_hashtable_insert(table, (void*)(key), SLS_STRING_LENGTH, &sample_int);

  int *ptr = (int*)(sls_hashtable_find(table, (void*)key, SLS_STRING_LENGTH));
  EXPECT_TRUE(ptr);
  if (ptr) {
    EXPECT_EQ(sample_int, *ptr);
  }
}

TEST_F(TableTests, Replace)
{
  auto const key = "hello";
  auto sample_intb = -1;
  sls_hashtable_insert(table, (void*)(key), SLS_STRING_LENGTH, &sample_int);
  sls_hashtable_insert(table, (void*)(key), SLS_STRING_LENGTH, &sample_intb);

  int *val = (int*)sls_hashtable_find(table, (void*)key, SLS_STRING_LENGTH);
  EXPECT_TRUE(val);
  if (val) {
    EXPECT_EQ(sample_intb, *val);
  }


}
