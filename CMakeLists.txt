cmake_minimum_required(VERSION 3.1)

project(dangerengine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fblocks")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

find_package(OpenGL)

add_subdirectory(extern/googlemock)
add_subdirectory(extern/glfw)
add_subdirectory(extern/kazmath)

set(DANGER_SRC
    src/slscontext.h
    src/slscontext.c
    src/slsutils.c
    src/slsloop.c)

set (DANGER_TEST_SRC
    tests/c_cases.c
    tests/main.cc)

add_library(dangerengine ${DANGER_SRC})

set(DANGER_DEPS
    glfw
    kazmath
    ${GLFW_LIBRARIES}
    ${OPENGL_gl_LIBRARY})



include_directories(
    ${GLFW_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/extern/kazmath
    ${CMAKE_SOURCE_DIR}/extern/googlemock/include
    ${CMAKE_SOURCE_DIR}/extern/gtest/include)


target_link_libraries(dangerengine
    ${DANGER_DEPS})

add_executable(dangerbird_bin src/exec.c)
target_link_libraries(dangerbird_bin
    dangerengine
    ${DANGER_DEPS})

add_executable(dangerbird_test ${DANGER_TEST_SRC})
target_link_libraries(dangerbird_test
    dangerengine
    gmock
    gmock_main
    gtest
    gtest_main)
