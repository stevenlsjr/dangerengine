cmake_minimum_required(VERSION 3.1)

project(dangerengine)


# copy resources
file(COPY ${CMAKE_SOURCE_DIR}/resources/
    DESTINATION ${CMAKE_BINARY_DIR}/)




set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fblocks")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

add_subdirectory(extern/kazmath)

set(DANGER_DEPS kazmath)

if (NOT EMSCRIPTEN)
  add_subdirectory(extern/googlemock)
  add_subdirectory(extern/glfw)


  find_package(OpenGL)
  find_package(GLEW)
  set(DANGER_DEPS
    ${DANGER_DEPS}
    glfw
    ${GLFW_LIBRARIES}
    ${OPENGL_gl_LIBRARY})

  if (GLEW_FOUND)
    set (DANGER_DEPS
      ${DANGER_DEPS}
      ${GLEW_LIBRARIES})
    include_directories(
      ${GLEW_INCLUDE_DIR}
      ${GLFW_SOURCE_DIR}/include
      ${OPENGL_INCLUDE_DIR})
  endif(GLEW_FOUND)

else (NOT EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FULL_ES2=1 -s USE_GLFW=3")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g4")
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} --pre-js ${CMAKE_SOURCE_DIR}/em.js.pre")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} --post-js ${CMAKE_SOURCE_DIR}/em.js.post")

  set(CMAKE_C_DEBUG_FLAGS "${CMAKE_C_FLAGS} -g4 -Wno-warn-absolute-paths")
endif (NOT EMSCRIPTEN)


set(DANGER_SRC
    src/dangerengine.h
    src/sls-gl.h
    src/slscontext.h
    src/slscontext.c
    src/slsutils.c
    src/slsutils.h
    src/sls-handlers.c
    src/sls-handlers.h
    src/math/mathmacs.h
    src/math/mathmacs.c
    src/data-types/array.h
    src/data-types/array.c
    src/data-types/btree.h
    src/data-types/btree.c)

set (DANGER_CTEST_SRC
    tests/c_cases.c
    tests/c_cases.h)

set (DANGER_TEST_SRC
    tests/minunit-runner.cc
    tests/btree_test.cc
    tests/math_tests.cc
    tests/main.cc)

include_directories(
    
    ${CMAKE_SOURCE_DIR}/extern/kazmath
    ${CMAKE_SOURCE_DIR}/extern/googlemock/include
    ${CMAKE_SOURCE_DIR}/extern/gtest/include)

add_library(dangerengine ${DANGER_SRC})

target_link_libraries(dangerengine
    ${DANGER_DEPS})

add_executable(dangerbird_bin src/exec.c)
target_link_libraries(dangerbird_bin
    dangerengine
    ${DANGER_DEPS})


if (NOT EMSCRIPTEN)
# add a separate library for tests compiled in C. Used for supporting build
# systems which don't compile C and C++ code separately
add_library(dangerbird_c_tests ${DANGER_CTEST_SRC})
target_link_libraries(dangerbird_c_tests dangerengine)

add_executable(dangerbird_test ${DANGER_TEST_SRC})
target_link_libraries(dangerbird_test
    dangerengine
    dangerbird_c_tests
    ${DANGER_DEPS}
    gtest
    gtest_main)

endif (NOT EMSCRIPTEN)
