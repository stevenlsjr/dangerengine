//
// Created by steve on 6/6/17.
//

#ifndef DANGERENGINE_DATA_TESTS_H
#define DANGERENGINE_DATA_TESTS_H

#include <dangerengine.h>
#include "../testing.h"

using namespace sls::testing;

class DataTests: public TestFixture {
public:
  int i = 100;
protected:

  void setup() override
  {

  }

  void teardown() override
  {

  }
};

static void data_tests_main(){
  DataTests fix;
  add_test_func("/data/sanity", [](auto fix){
    printf("hello!");
    auto tfix = dynamic_cast<DataTests*>(fix);
    g_assert_true(tfix->i == 100);
  });
}

#endif //DANGERENGINE_DATA_TESTS_H
