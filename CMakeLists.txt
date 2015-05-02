cmake_minimum_required(VERSION 3.1)

project(dangerengine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fblocks")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

find_package(OpenGL)
find_package(GLEW)

add_subdirectory(extern/googlemock)
add_subdirectory(extern/glfw)
add_subdirectory(extern/kazmath)

set(DANGER_SRC
    src/slscontext.h
    src/slscontext.c
    src/slsutils.c
    src/slsutils.h
    src/slsloop.c
    src/slsloop.h
    src/sls-handlers.c
    src/sls-handlers.h
    src/dangerengine.h
    src/math/mathmacs.h
    )

set (DANGER_CTEST_SRC
    tests/c_cases.c
    tests/c_cases.h)

set (DANGER_TEST_SRC
    tests/minunit-runner.cc
    tests/main.cc)

add_library(dangerengine ${DANGER_SRC})

set(DANGER_DEPS
  glfw
  kazmath
  ${GLFW_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  )



include_directories(
    ${GLFW_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/extern/kazmath
    ${CMAKE_SOURCE_DIR}/extern/googlemock/include
    ${CMAKE_SOURCE_DIR}/extern/gtest/include)



if (GLEW_FOUND)
  set (DANGER_DEPS
    ${DANGER_DEPS}
    ${GLEW_LIBRARIES})
  include_directories(${GLEW_INCLUDE_DIR})
endif(GLEW_FOUND)

target_link_libraries(dangerengine
    ${DANGER_DEPS})

add_executable(dangerbird_bin src/exec.c)
target_link_libraries(dangerbird_bin
    dangerengine
    ${DANGER_DEPS})


# add a separate library for tests compiled in C. Used for supporting build
# systems which don't compile C and C++ code separately
add_library(dangerbird_c_tests ${DANGER_CTEST_SRC})
target_link_libraries(dangerbird_c_tests dangerengine)

add_executable(dangerbird_test ${DANGER_TEST_SRC})
target_link_libraries(dangerbird_test
    dangerengine
    dangerbird_c_tests
    gtest
    gtest_main)
