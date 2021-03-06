# -*- cmake;company -*-


cmake_minimum_required(VERSION 3.1)
project(dangerengine)

# Platform Info macros

if (CMAKE_SYSTEM_NAME EQUAL Linux)
  set(LINUX)
elseif (CMAKE_SYSTEM_NAME EQUAL FreeBSD)
  set(FREEBSD)
endif ()


set(DANGERENGINE_BUILD_TESTS ON
    CACHE BOOL
    "build unit tests for dangerengine")

set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake/" CACHE STRING "cmake  module  path")

set(DANGERENGINE_EXE "demo")

#--------------------------------------------
#--			package dependencides
#--------------------------------------------

set(CMAKE_PREFER_PTHREADS ON FORCE)


include(ExternalProject)
find_package(OpenGL)

find_package(Doxygen)

if (EMSCRIPTEN)
  # we don't need pkg-config, but unit tests
  # are disabled
  set(DANGERBIRD_EMSCRIPTEN ON)

  set(CMAKE_C_FLAGS
      "-s USE_SDL=2 ${EM_USE_PTHREADS}" ${CMAKE_C_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS
      "-s USE_SDL=2 --pre-js '${CMAKE_SOURCE_DIR}/src/emscripten/module.pre.js'")
  set(CMAKE_SHARED_LINKER_FLAGS
      "-s USE_SDL=2")

  set(CMAKE_C_FLAGS_DEBUG
      # enable source maps
      "-O0 -g4")

  set(CMAKE_EXECUTABLE_SUFFIX ".js")

else ()
  find_package(PkgConfig REQUIRED)

  find_package(Threads)
endif ()


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
    src/renderer/slssprite.h
    src/renderer/slssprite.c

    src/sls-commonlibs.h
    src/sls-gl.h
    src/slsutils.h
    src/slsutils.c
    src/sls-imagelib.h
    src/slscontext.c
    src/slscontext.h
    src/slsmacros.h
    src/state/slsInputState.c
    src/state/slsInputState.h
    src/slserror.c src/slserror.h src/renderer/constants.cc src/math/math-operations.h)


#tests  compiled  in  c++
set(DANGER_TEST_SRC
    extern/Unity/src/unity.c
    extern/Unity/src/unity.h
    extern/Unity/src/unity_internals.h
    tests/main-tests.c
    tests/data-types/data-tests.c
    tests/math-tests.c)


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


#kazmath
set(KAZMATH_BUILD_JNI_WRAPPER OFF CACHE BOOL "" FORCE)
set(KAZMATH_BUILD_LUA_WRAPPER OFF CACHE BOOL "" FORCE)
set(KAZMATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/kazmath)


add_subdirectory(src/data-types)


# cmake config file
configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

set(CMAKE_LINK_LIBRARY_FLAGS "${CMAKE_LINK_LIBRARY_FLAGS}  ${SDL2_LDFLAGS} ")
set(CMAKE_LINK_EXE_FLAGS "${CMAKE_LINK_EXE_FLAGS}  ${SDL2_LDFLAGS}")
set(DANGER_DEPS
    ${DANGERTYPES_LIB}
    ${DANGER_PTHREAD_LIB}
    ${OPENGL_LIBRARIES}

    kazmath)


if (NOT EMSCRIPTEN)
  message("setting dependencies")
  set(DANGER_DEPS ${DANGER_DEPS}

      ${SDL2_LIBRARIES})
endif ()

if (LINUX)
  set(DANGER_DEPS ${DANGER_DEPS} m dl)
endif ()


link_directories(${SDL2_LIBRARY_DIRS}
                 ${SDL2IMG_LIBRARY_DIRS}
                 ${GLIB_LIBRARY_DIRS})

include_directories(
  ${CMAKE_SOURCE_DIR}/extern/lib
  ${CMAKE_SOURCE_DIR}/extern/kazmath
  ${CMAKE_SOURCE_DIR}/extern/glad_debug/include
  ${CMAKE_SOURCE_DIR}/extern/stb
  ${CMAKE_SOURCE_DIR}/extern/Unity/src
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/data-types
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIRS}
  ${GLIB_INCLUDE_DIRS}
)


get_property(INCLUDE_DIRECTORIES
             DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             PROPERTY INCLUDE_DIRECTORIES)

message("include dirs: ${INCLUDE_DIRECTORIES}")

#---------------------------------------
#  build  dangerengine
#---------------------------------------

set(BUILD_SHARED_LIBS ON)
add_library(dangerengine STATIC ${DANGER_SRC})


target_link_libraries(dangerengine
                      ${DANGER_DEPS})

get_target_property(DANGERENGINE_OUTPUT dangerengine LIBRARY_OUTPUT_NAME)


#  copy  resources  on  build

add_custom_command(
  TARGET sls_resources PRE_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR} &&
  ${CMAKE_COMMAND} -E cmake_echo_color --green
  "--  copying  resources/  to  ${CMAKE_BINARY_DIR}"
  COMMENT "")

add_executable(${DANGERENGINE_EXE}
               ${DANGER_DEMO_SRC})


add_dependencies(${DANGERENGINE_EXE}
                 dangerengine
                 sls_resources)

target_link_libraries(${DANGERENGINE_EXE}
                      dangerengine
                      ${DANGER_DEPS})


#  add  a  separate  library  for  tests  compiled  in  C.  Used  for  supporting  build
#  systems  which  don't  compile  C  and  C++  code  separately


# don't use googletest when emscripten is the platform
message("build tests ${DANGERENGINE_BUILD_TESTS}")

if (DANGERENGINE_BUILD_TESTS)
  add_executable(tests ${DANGER_TEST_SRC})
  target_link_libraries(tests
                        dangerengine
                        ${DANGER_DEPS})
  add_dependencies(tests
                   sls_resources)

endif ()


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
