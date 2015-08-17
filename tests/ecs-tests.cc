//
// Created by Steven on 8/6/15.
//

#include <gtest/gtest.h>
#include <dangerengine.h>

class EscTests : public ::testing::Test {

protected:

  slsEntity *entity;

  virtual void SetUp()
  {
    entity = new slsEntity();
    sls_msg(entity, init);
  }

  virtual void TearDown()
  {
    if (entity) {
      delete(sls_msg(entity, dtor));
    }
  }

};
