cmake_minimum_required(VERSION 2.8)

project(dangerengine)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" CACHE STRING "cmake module path")

#--------------------------------------------
#-------------source lists-------------------
#--------------------------------------------
set(DANGER_SRC
  src/dangerengine.h

# context handling and general utilities
  src/sls-gl.h
  src/sls-imagelib.h
  src/slscontext.h
  src/slscontext.c
  src/slsutils.c
  src/slsutils.h
  src/contexthandlers.c
  src/contexthandlers.h
  src/slserrcode.c
  src/slserrcode.h

  src/math/mathmacs.h
  src/math/mathmacs.c
  src/math/math-types.c
  src/math/math-types.h
  src/math/slsTransform2D.c
  src/math/slsTransform2D.h
  src/math/slsMatrixStack.c
  src/math/slsMatrixStack.h

#data types
  src/data-types/callbacks.h
  src/data-types/callbacks.c
  src/data-types/array.h
  src/data-types/array.c
  src/data-types/ptrarray.h
  src/data-types/ptrarray.c
  src/data-types/linkedlist.h
  src/data-types/linkedlist.c
  src/data-types/hashtable.c
  src/data-types/hashtable.h

#renderer types
  src/renderer/slsmesh.c
  src/renderer/slsmesh.h
  src/renderer/slsmodel.c
  src/renderer/slsmodel.h
  src/renderer/slstexture.c
  src/renderer/slstexture.h
  src/renderer/slsshader.c
  src/renderer/slsshader.h
  src/renderer/shaderutils.c
  src/renderer/shaderutils.h
  src/renderer/slsSpriteRenderer.c
  src/renderer/slsSpriteRenderer.h
  src/state/slsSprite.c
  src/state/slsSprite.h

# game state machine
  src/state/slsEntity.h
  src/state/slsEntity.c

  src/state/slsAppState.c
  src/state/slsAppState.h
  src/state/slsEntityDraw.c
  src/state/slsEntityDraw.h
  src/math/slsCamera.c
  src/math/slsCamera.h src/state/slsBehavior.c src/state/slsBehavior.h src/state/resourceutils.c src/state/resourceutils.h)


# tests compiled in c
set(DANGER_CTEST_SRC
  tests/c_cases.c
  tests/c_cases.h)

#tests compiled in c++
set(DANGER_TEST_SRC
  tests/minunit-runner.cc
  tests/math_tests.cc
  tests/main.cc
  tests/utiltests.cc
  tests/ecs-tests.cc
  tests/render_tests.cc
  tests/image_tests.cc
  tests/hash-tests.cc
  tests/ptrarray-tests.cc
  tests/array-tests.cc tests/list-tests.cc)


set(DANGER_DEMO_SRC
  demos/exec.c
  demos/render_demo.c
  demos/render_demo.h demos/slsTank.c demos/slsTank.h)

#--------------------------------------------
#-------------project config-----------------
#--------------------------------------------


#copy resources to build
file(COPY ${CMAKE_SOURCE_DIR}/resources/
  DESTINATION ${CMAKE_BINARY_DIR}/resources)


file(GLOB DANGER_RES ${CMAKE_SOURCE_DIR}
  resources/*
  resources/**/*
  CMakeLists.txt
  em.js.*
  *.cmake)

add_custom_target(sls_resources
  COMMAND
  ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/ &&
  ${CMAKE_COMMAND} -E cmake_echo_color --green
  "--    copying resources/ to ${CMAKE_BINARY_DIR}"
  COMMENT "application resources"
  SOURCES ${DANGER_RES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions -fblocks")
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")


#kazmath
add_subdirectory(extern/kazmath)
set(KAZMATH_BUILD_TESTS OFF)
set(KAZMATH_BUILD_JNI_WRAPPER OFF)
set(KAZMATH_BUILD_GL_UTILS OFF)


include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/extern/kazmath

  ${CMAKE_SOURCE_DIR}/extern/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/gtest/include)

#objc bindings
add_subdirectory(src/objc)

set(DANGER_DEPS kazmath)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Doxygen)

find_package(PkgConfig REQUIRED)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

pkg_check_modules(APR apr-1)

set(CMAKE_LIBRARY_PATH
  "${CMAKE_LIBRARY_PATH} ${SDL2_LIBRARY_DIRS} ${SDL2IMG_LIBRARY_DIRS}"
  CACHE STRING "library search path")


#only compile gtest and glfw when not using emscripten
add_subdirectory(extern/googlemock)

find_package(DevIL)

set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} ${SDL2_LDFLAGS}")
set(DANGER_DEPS
  ${DANGER_DEPS}
  ${GLEW_LIBRARY}
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  ${APR_LIBRARIES})


message("${DANGER_DEPS}")
message("${SDL2_IMAGE_LIBRARIES}")

include_directories(
  ${GLEW_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR})



#---------------------------------------
# build dangerengine
#---------------------------------------

add_library(dangerengine STATIC ${DANGER_SRC})

add_dependencies(dangerengine
  kazmath)

target_link_libraries(dangerengine
  ${DANGER_DEPS})

#---------------------------------------
# objective c library
#---------------------------------------


if (SLS_USE_OBJC)
  add_executable(demo_objc demos/objc-demo.m)
  target_link_libraries(demo_objc
    dangerengine-fw)
  add_dependencies(demo_objc dangerengine-fw sls_resources)
endif (SLS_USE_OBJC)


# copy resources on build

add_custom_command(
  TARGET sls_resources PRE_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/ &&
  ${CMAKE_COMMAND} -E cmake_echo_color --green
  "--    copying resources/ to ${CMAKE_BINARY_DIR}"
  COMMENT "")

add_executable(demo
  ${DANGER_DEMO_SRC})

add_dependencies(demo
  dangerengine
  sls_resources)

target_link_libraries(demo
  dangerengine
  ${DANGER_DEPS})




# add a separate library for tests compiled in C. Used for supporting build
# systems which don't compile C and C++ code separately
add_library(c_tests ${DANGER_CTEST_SRC})
add_dependencies(c_tests dangerengine)
target_link_libraries(c_tests dangerengine)

add_executable(tests ${DANGER_TEST_SRC})
add_dependencies(tests
  gtest
  gtest_main
  dangerengine)
target_link_libraries(tests
  dangerengine
  c_tests
  ${DANGER_DEPS}
  gtest
  gtest_main)


if (SLS_USE_OBJC)
  add_dependencies(c_tests dangerengine-fw)
  add_dependencies(tests dangerengine-fw)

  target_link_libraries(c_tests dangerengine-fw)
  target_link_libraries(tests dangerengine-fw)

  foreach (i
    dangerengine
    dangerengine-fw
    demo
    demo_objc
    c_tests
    tests)

    set_property(TARGET ${i}
      APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

  endforeach (i)

endif (SLS_USE_OBJC)

if (DOXYGEN_FOUND)

  set(DANGER_DOC_FILES
    "${CMAKE_SOURCE_DIR}/src/                     \
    ${CMAKE_SOURCE_DIR}/src/data-types/           \
    ${CMAKE_SOURCE_DIR}/src/math/                 \
    ${CMAKE_SOURCE_DIR}/extern/glfw/include/GLFW \
    ${CMAKE_SOURCE_DIR}/extern/kazmath/kazmath   \
    ${CMAKE_SOURCE_DIR}/extern/kazmath/kazmath/GL")


  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)


endif (DOXYGEN_FOUND)
