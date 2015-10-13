//
// Created by Steven on 6/23/15.
//

#ifndef DANGERENGINE_TEST_UTILS_H
#define DANGERENGINE_TEST_UTILS_H

#include <gtest/gtest.h>
#include <functional>
#include <stdio.h>

namespace sls {

/**
 * @brief executes a function with stderr redirected to dev/null
 * @detail used for gltex testing to silence expected warning logs
 * note: this is only intened for c stdio functions. I have not tested it
 * for silencing c++ std::cerr streams
 *
 * @param fn function, function-object, or lambda for use in execution
 */

#ifndef WIN32
static inline
void silence_stderr(std::function<void(void)> fn)
{

  constexpr auto devnull = "/dev/null";
  auto _stderr = stderr;

  stderr = fopen(devnull, "w");
  if (!stderr) {
    stderr = _stderr;
    fn();
  } else {
    fn(); // execute function

    fclose(stderr);
    stderr = _stderr;
  }
}

#else
static inline
void silence_stderr(std::function<void(void)> fn)
{

}
#endif

}

#endif //DANGERENGINE_TEST_UTILS_H
