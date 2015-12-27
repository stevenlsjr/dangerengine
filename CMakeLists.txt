cmake_minimum_required(VERSION 2.8)

project(dangerengine)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" CACHE STRING "cmake module path")

#--------------------------------------------
#-------------source lists-------------------
#--------------------------------------------


file(REMOVE ${CMAKE_SOURCE_DIR}/src/sls-build-dir.c)
configure_file(
  ${CMAKE_SOURCE_DIR}/src/sls-build-dir.c.in
  ${CMAKE_SOURCE_DIR}/src/sls-build-dir.c @ONLY)

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
  src/renderer/slsmaterial.c
  src/renderer/slsmaterial.h
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
  src/math/slsCamera.h
  src/state/slsBehavior.c
  src/state/slsBehavior.h
  src/state/resourceutils.c
  src/state/resourceutils.h
  src/data-types/intrusivelist.h
  src/renderer/slsTTFText.c
  src/renderer/slsTTFText.h
  src/sls-commonlibs.h
  src/state/slsPhysics.c
  src/state/slsPhysics.h
  src/math/slsTransform.c
  src/math/slsTransform.h
  src/sls-build-dir.c
  src/sls-build-dir.h
  src/renderer/slsLight.c src/renderer/slsLight.h
  src/cppapi.h
  src/math/slsTrackball.c src/math/slsTrackball.h
  src/math/slsTransform3D.c src/math/slsTransform3D.h src/math/slsTransform_private.h src/renderer/slsLocationTable.c src/renderer/slsLocationTable.c src/renderer/slsLocationTable.h)


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
  tests/array-tests.cc
  tests/list-tests.cc tests/LocationTableTests.cc)


set(CLIENT_SRC
  src/client/main.c
  src/client/ClientContext.c src/client/ClientContext.h)

set(DANGER_DEMO_SRC
  demos/exec.c
  demos/earth.c demos/earth.h demos/sphere.c demos/sphere.h)

#--------------------------------------------
#-------------project config-----------------
#--------------------------------------------

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/extern/kazmath

  ${CMAKE_SOURCE_DIR}/extern/Chipmunk2D/include

  ${CMAKE_SOURCE_DIR}/extern/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/gtest/include
  ${APR_UTIL_INCLUDE_DIRS}
  /usr/local/include/
  /usr/local/Cellar/apr/1.5.1/libexec/include/apr-1
  /usr/local/Cellar/apr-util/1.5.4/include/apr-1)


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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -fblocks")


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions -fblocks")
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")


#kazmath
set(KAZMATH_BUILD_JNI_WRAPPER OFF FORCE)
set(KAZMATH_BUILD_TESTS OFF FORCE)
add_subdirectory(extern/kazmath)


#chipmunk
add_subdirectory(extern/Chipmunk2D)


set(DANGER_DEPS kazmath chipmunk_static)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Doxygen)

find_package(PkgConfig REQUIRED)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

pkg_check_modules(APR apr-1)
pkg_check_modules(APR_UTIL apr-util-1)

set(CMAKE_LIBRARY_PATH
  "${CMAKE_LIBRARY_PATH}  \
  ${SDL2_LIBRARY_DIRS}  \
  ${SDL2IMG_LIBRARY_DIRS}"
  CACHE STRING "library search path")


#only compile gtest and glfw when not using emscripten
add_subdirectory(extern/googlemock)

set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} ${SDL2_LDFLAGS}")
set(DANGER_DEPS
  ${DANGER_DEPS}
  ${GLEW_LIBRARY}
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  ${APR_LIBRARIES}
  ${APR_UTIL_LIBRARIES}
  )


message("apr: ${APR_INCLUDE_DIRS}")

include_directories(
  ${GLEW_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${APR_INCLUDE_DIRS}
  ${APR_UTIL_INCLUDE_DIRS}
  /usr/local/Cellar/apr/1.5.1/libexec/include/apr-1
  /usr/local/Cellar/apr-util/1.5.4/include/apr-1)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

foreach (dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach ()


#---------------------------------------
# build dangerengine
#---------------------------------------

set(BUILD_SHARED_LIBS ON)
add_library(dangerengine ${DANGER_SRC})

add_dependencies(dangerengine
  kazmath)

target_link_libraries(dangerengine
  ${DANGER_DEPS})

get_target_property(DANGERENGINE_OUTPUT dangerengine LIBRARY_OUTPUT_NAME)

add_executable(client
  ${CLIENT_SRC})

target_link_libraries(client dangerengine)

#---------------------------------------
# objective c library
#---------------------------------------


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
