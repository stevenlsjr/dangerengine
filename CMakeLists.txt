# -*- cmake;company -*-


cmake_minimum_required(VERSION 3.1)
project(dangerengine)

set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake/" CACHE STRING "cmake  module  path")


#--------------------------------------------
#--			package dependencides
#--------------------------------------------

set(CMAKE_PREFER_PTHREADS ON FORCE)

find_package(OpenGL)
find_package(Threads REQUIRED)
find_package(GLEW)
find_package(Doxygen)

find_package(PkgConfig REQUIRED)

find_package(SDL2)
find_package(SDL2_image)
pkg_search_module(GLIB glib-2.0 REQUIRED)


#force  pthreads  on  win32

if (${WIN32})
  message(STATUS "win32 pthread config")

  set(DANGERER_PTHREAD_INCLUDE_DIR PTHREAD_INCLUDE_DIR_NOTFOUND
      CACHE PATH "path to pthread include folder")

  set(DANGERER_PTHREAD_LIB PTHREAD_LIB_NOTFOUND
      CACHE FILEPATH "pthread library file")

  include_directories(${DANGERER_PTHREAD_INCLUDE_DIR})

endif (${WIN32})


#--------------------------------------------
#-------------source  lists-------------------
#--------------------------------------------

add_subdirectory(extern/lib)
set(DANGER_SRC
    ${VENDOR_SRC}
    src/contexthandlers.c
    src/contexthandlers.h
    src/dangerengine.h
    src/data-types/dangertypes.h
    src/math/math-types.c
    src/math/math-types.h
    src/math/slsMathUtils.c
    src/math/slsMathUtils.h
    src/math/slsSimd.c
    src/math/slsSimd.h
    src/math/slsTrackball.c
    src/math/slsTrackball.h
    src/renderer/shaderutils.c
    src/renderer/shaderutils.h
    src/renderer/slsmesh.c
    src/renderer/slsmesh.h
    src/renderer/slsrender.c
    src/renderer/slsrender.h
    src/renderer/slsshader.c
    src/renderer/slsshader.h
    src/sls-commonlibs.h
    src/sls-gl.h
    src/sls-imagelib.h
    src/slscontext.c
    src/slscontext.h
    src/slsmacros.h
    src/state/slsAppState.c
    src/state/slsAppState.h
    src/state/slsInputState.c
    src/state/slsInputState.h
    )


#tests  compiled  in  c++
set(DANGER_TEST_SRC
    tests/data-types/array-tests.cc
    tests/data-types/hash-tests.cc
    tests/data-types/list-tests.cc
    tests/data-types/ptrarray-tests.cc
    tests/main.cc
    tests/math_tests.cc
    tests/utiltests.cc
    )


set(DANGER_DEMO_SRC
    demos/demo_2.c)

#--------------------------------------------
#-------------project  config-----------------
#--------------------------------------------


#copy  resources  to  build
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
    "--  copying  resources/  to  ${CMAKE_BINARY_DIR}"
    COMMENT "application  resources"
    SOURCES ${DANGER_RES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -std=gnu11")

add_subdirectory(extern/Chipmunk2D)
add_subdirectory(extern/kazmath)
add_subdirectory(src/data-types)

#kazmath
set(KAZMATH_BUILD_JNI_WRAPPER OFF CACHE BOOLEAN "kazmath build tests")
set(KAZMATH_BUILD_TESTS OFF CACHE BOOLEAN "kazmath build tests")

#chipmunk
set(BUILD_DEMOS OFF FORCE)

# cmake config file
configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

set(CMAKE_LINK_LIBRARY_FLAGS "${CMAKE_LINK_LIBRARY_FLAGS}  ${SDL2_LDFLAGS} ${GLIB_LDFLAGS}")
set(CMAKE_LINK_EXE_FLAGS "${CMAKE_LINK_EXE_FLAGS}  ${SDL2_LDFLAGS} ${GLIB_LDFLAGS}")
set(DANGER_DEPS
    ${DANGERTYPES_LIB}
    ${DANGER_PTHREAD_LIB}
    ${GLEW_LIBRARY}
    ${GLIB_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
    kazmath
    )

if (${UNIX})
  set(DANGER_DEPS ${DANGER_DEPS} m)
endif ()


link_directories(${SDL2_LIBRARY_DIRS}
    ${SDL2IMG_LIBRARY_DIRS}
    ${GLIB_LIBRARY_DIRS})

include_directories(
    ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/extern/lib
    ${CMAKE_SOURCE_DIR}/extern/kazmath
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/data-types
    ${CMAKE_BINARY_DIR}/src
    ${SDL2_IMAGE_INCLUDE_DIRS}/SDL2
    ${SDL2_INCLUDE_DIRS}/SDL2
    ${APR_INCLUDE_DIRS}
    ${APR_UTIL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
)


#---------------------------------------
#  build  dangerengine
#---------------------------------------

set(BUILD_SHARED_LIBS ON)
add_library(dangerengine STATIC ${DANGER_SRC})

add_dependencies(dangerengine
    kazmath)

target_link_libraries(dangerengine
    ${DANGER_DEPS})

get_target_property(DANGERENGINE_OUTPUT dangerengine LIBRARY_OUTPUT_NAME)


#  copy  resources  on  build

add_custom_command(
    TARGET sls_resources PRE_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/ &&
    ${CMAKE_COMMAND} -E cmake_echo_color --green
    "--  copying  resources/  to  ${CMAKE_BINARY_DIR}"
    COMMENT "")

add_executable(demo
    ${DANGER_DEMO_SRC})


add_dependencies(demo
    dangerengine
    sls_resources)

target_link_libraries(demo
    dangerengine
    ${DANGER_DEPS})


#  add  a  separate  library  for  tests  compiled  in  C.  Used  for  supporting  build
#  systems  which  don't  compile  C  and  C++  code  separately


# don't use googletest when emscripten is the platform
add_subdirectory(extern/googletest/googlemock)

add_executable(tests ${DANGER_TEST_SRC})
add_dependencies(tests dangerengine)
target_link_libraries(tests
    dangerengine
    ${DANGER_DEPS}
    gtest
    gtest_main)


if (DOXYGEN_FOUND)
  set(DANGER_DOC_FILES
      "${CMAKE_SOURCE_DIR}/src \
      ${CMAKE_SOURCE_DIR}/src/data-types \
      ${CMAKE_SOURCE_DIR}/src/math \
      ${CMAKE_SOURCE_DIR}/src/renderer \
      ${CMAKE_SOURCE_DIR}/src/state"
      )


  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(doc
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating  API  documentation  with  Doxygen" VERBATIM)
  add_custom_target(view-html-docs
      open ./html/index.html
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "opening  html  docs")

  add_dependencies(view-html-docs doc)


endif (DOXYGEN_FOUND)
