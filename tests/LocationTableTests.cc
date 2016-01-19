/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/26/15, Steven
 * 
 **/


#include <gmock/gmock.h>
#include <dangerengine.h>
#include <vector>

using namespace std;

class LocationTableTest : public ::testing::Test {

protected:
  slsLocationTable table;

  vector<pair<string, GLuint>> pairs{
      make_pair("foo", 100),
      make_pair("bar", 200),
  };

  virtual void SetUp()
  {
    sls_locationtable_init(&table);

  }

  virtual void TearDown()
  {

    sls_locationtable_dtor(&table);


  }
};

static inline
std::string to_string(pair<string, GLuint> const &p)
{
  return (std::stringstream()
          << "{.first= \""
          << p.first
          << "\", .second="
          << p.second
          << "}").str();
}


TEST_F(LocationTableTest, Set)
{

  for (auto const &p: pairs) {
    auto res = sls_locationtable_set(&table, p.first.c_str(), p.second);
    EXPECT_TRUE(res);
    if (res) {
      EXPECT_EQ(p.second, *res);
    }

  }
}


TEST_F(LocationTableTest, Get)
{

  auto counter = 0;
  for (auto const &p: pairs) {
    auto res = sls_locationtable_set(&table, p.first.c_str(), p.second);
    auto get = sls_locationtable_get(&table, p.first.c_str());
    EXPECT_TRUE(res && get) << "item " << counter << ", " << to_string(p);
    if (res && get) {
      EXPECT_EQ(*res, *get) << "item " << counter << ", " << to_string(p);
    }
    counter++;
  }

}