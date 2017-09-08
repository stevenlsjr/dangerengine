//
// Created by steve on 9/7/17.
//

#ifndef DANGERENGINE_TESTING_H
#define DANGERENGINE_TESTING_H

#include <functional>

namespace sls::testing {
class TestFixture;
using TestFunc = std::function<void(TestFixture *)>;


class TestFixture {
protected:
  virtual void setup() {}

  virtual void teardown(){}
public:

  static void setup_fn(TestFixture *fix, const void *data) {
    fix->setup();

  }
  static void teardown_fn(TestFixture *fix, const void *data) {
    fix->teardown();
  }
};




static void add_test_func(char const *path, TestFunc &&fn){
  const void *data = reinterpret_cast<void*>(&fn);
  const auto test_fn = [](TestFixture *fixture, gconstpointer test_data) {
    TestFunc const *original_fn = static_cast<TestFunc const *>(test_data);
    (*original_fn)(fixture);


  };
  g_test_add(path, TestFixture, data, TestFixture::setup_fn, test_fn, TestFixture::teardown_fn);

}

}

#endif //DANGERENGINE_TESTING_H
