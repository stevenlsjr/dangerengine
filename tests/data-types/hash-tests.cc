//
// Created by Steven on 7/22/15.
//

#include <climits>
#include <dangerengine.h>
#include <gtest/gtest.h>
#include <map>
#include <random>
#include <utility>

using namespace std;

class TableTests : public ::testing::Test
{
protected:
  slsHashTable* table;

  size_t array_size = 100;

  map<string, int> hash_data = { { "hello", -1 },
                                 { "world", 100 },
                                 { "foo", 99 },
                                 { "bar", 10000000 } };

  virtual void SetUp()
  {

    slsCallbackTable key_cback = {}, val_cback = {};

    key_cback.copy_fn = [](void const* data) {
      auto str = (char* const)data;
      return static_cast<void*>(strdup(str));
    };

    key_cback.free_fn = [](void* data) {
      if (data) {
        free(data);
      } else {
        FAIL() << "invalid key free";
      }
    };

    key_cback.cmp_fn = sls_cmp_string;
    key_cback.copy_fn = [](auto k) {
      return static_cast<void*>(strdup((char const*)k));
    };
    key_cback.free_fn = free;

    val_cback.cmp_fn = sls_cmp_uintptr;

    val_cback.copy_fn = [](auto v) {
      int* p = static_cast<int*>(malloc(sizeof(int)));
      *p = *static_cast<int const*>(v);
      return static_cast<void*>(p);
    };

    val_cback.free_fn = free;

    table = sls_hashtable_init(new slsHashTable, array_size,
                               sls_hash_fn_default, &key_cback, &val_cback);

    ASSERT_EQ(0, table->key_callbacks.cmp_fn("hello", "hello"))
      << "sanity checking cmp_fn";

    for (auto const& p : hash_data) {
      auto key = p.first.c_str();
      auto val = p.second;
      ASSERT_TRUE(
        sls_hashtable_insert(table, (void*)(key), SLS_STRING_LENGTH, &val))
        << "table insertion failure";
    }

    ASSERT_TRUE(table);
  }

  virtual void TearDown() { delete (sls_hashtable_dtor(table)); }
};

static inline std::string
to_string(pair<string, int> const& p)
{
  std::stringstream s;
  s << "{.first= \"" << p.first << "\", .second=" << p.second << "}";
  return s.str();
}

TEST_F(TableTests, FindSingleItem)
{
  auto key = "foo";
  auto val = hash_data[key];

  auto res = sls_hashtable_find(table, (void*)key, SLS_STRING_LENGTH);
  EXPECT_TRUE(res);
  if (res) {
    EXPECT_EQ(val, *static_cast<int const*>(res));
  }
}

TEST_F(TableTests, FindItems)
{
  // shuffle keys
  vector<pair<string, GLuint>> shuffled_data;

  for (auto const& p : hash_data) {
    shuffled_data.push_back(p);
  }

  auto rng = move(default_random_engine((random_device())()));

  shuffle(shuffled_data.begin(), shuffled_data.end(), rng);

  for (auto const& p : shuffled_data) {

    auto hval = sls_hashtable_find(table, p.first.c_str(), SLS_STRING_LENGTH);
    EXPECT_TRUE(hval);
    if (hval) {
      EXPECT_EQ(p.second, *static_cast<int const*>(hval));
    }
  }
}

TEST_F(TableTests, SearchNonExistingItem)
{
  auto key = string("I'm not in the hashtable");
  EXPECT_EQ(hash_data.end(), hash_data.find(key));

  auto res = sls_hashtable_find(table, key.c_str(), SLS_STRING_LENGTH);
  EXPECT_FALSE(res) << "res should be NULL";
}
