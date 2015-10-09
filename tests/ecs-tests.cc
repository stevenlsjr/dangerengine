//
// Created by Steven on 8/6/15.
//

#include <gtest/gtest.h>
#include <dangerengine.h>

class EscTests : public ::testing::Test {

protected:

  slsEntity *entity;
  apr_pool_t *pool;

  slsComponentMask mask =
      static_cast<slsComponentMask>(
          SLS_COMPONENT_BOUNDED |
          SLS_COMPONENT_KINETIC);
  std::string name = "myentity";

  virtual void SetUp()
  {
    apr_initialize();
    apr_pool_create(&pool, nullptr);
    entity = sls_entity_new(pool, mask, name.c_str());

  }

  virtual void TearDown()
  {
    sls_msg(entity, dtor);
    apr_terminate();
  }

};

TEST_F(EscTests, Mask)
{
  EXPECT_EQ(mask, entity->component_mask);
  EXPECT_TRUE(entity->component_mask & SLS_COMPONENT_BOUNDED);
  EXPECT_FALSE(entity->component_mask & SLS_COMPONENT_TEXTURE);
}


TEST_F(EscTests, Name)
{
  EXPECT_STREQ(name.c_str(), entity->name);
}
