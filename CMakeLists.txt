cmake_minimum_required (VERSION  3.4)
project (dangerengine)

set (CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/CMake/"  CACHE  STRING  "cmake  module  path")



#--------------------------------------------
#--			package dependencides
#--------------------------------------------

set (CMAKE_PREFER_PTHREADS  ON  FORCE)

find_package (OpenGL  REQUIRED)
find_package (Threads  REQUIRED)
find_package (GLEW  REQUIRED)
find_package (Doxygen)

#find_package (PkgConfig  REQUIRED)

find_package (SDL2  REQUIRED)
find_package (SDL2_image  REQUIRED)

#pkg_check_modules (APR  apr-1)
#pkg_check_modules (APR_UTIL  apr-util-1)

#force  pthreads  on  win32

if  (WIN32)
  message(STATUS "win32 pthread config")

  set (DANGERER_PTHREAD_INCLUDE_DIR PTHREAD_INCLUDE_DIR_NOTFOUND
    CACHE PATH "path to pthread include folder")

  set (DANGERER_PTHREAD_LIB PTHREAD_LIB_NOTFOUND
    CACHE FILEPATH "pthread library file")

  include_directories(${DANGERER_PTHREAD_INCLUDE_DIR})
  
endif (WIN32)

file (REMOVE  ${CMAKE_SOURCE_DIR}/src/sls-build-dir.c)
configure_file (
  src/sls-build-dir.c.in
  src/sls-build-dir.c  @ONLY)


  
#--------------------------------------------
#-------------source  lists-------------------
#--------------------------------------------

set (DANGER_SRC
  src/dangerengine.h

  #  context  handling  and  general  utilities
  src/sls-gl.h
  src/sls-imagelib.h
  src/slscontext.h
  src/slscontext.c
  src/slsutils.c
  src/slsutils.h
  src/contexthandlers.c
  src/contexthandlers.h

  src/cppapi.h
  src/slsmacros.h

  src/math/slsMathUtils.h
  src/math/slsMathUtils.c
  src/math/math-types.c
  src/math/math-types.h
  src/math/slsTrackball.c  src/math/slsTrackball.h
  src/math/slsCamera.c
  src/math/slsCamera.h

  #renderer  types
  src/renderer/slsmesh.c
  src/renderer/slsmesh.h
  src/renderer/slsmaterial.c
  src/renderer/slsmaterial.h
  src/renderer/slsshader.c
  src/renderer/slsshader.h
  src/renderer/shaderutils.c
  src/renderer/shaderutils.h

  #  game  state  machine

  src/state/resourceutils.c
  src/state/resourceutils.h

  src/sls-commonlibs.h

  src/sls-build-dir.c
  src/sls-build-dir.h
  src/renderer/slsLight.c  src/renderer/slsLight.h
  src/renderer/geometries.c
  src/renderer/geometries.h

  src/cxx/overloads.h
  src/data-types/dangertypes.h
  src/state/slsEvent.c  src/state/slsEvent.h
  src/math/slsSimd.c  src/math/slsSimd.h

  src/renderer/nuklear.c  src/renderer/sls-nuklear.h)


#  tests  compiled  in  c
set (DANGER_CTEST_SRC
  tests/c_cases.c
  tests/c_cases.h  src/cxx/cxx_interop.h)

#tests  compiled  in  c++
set (DANGER_TEST_SRC
  tests/minunit-runner.cc
  tests/math_tests.cc
  tests/main.cc
  tests/utiltests.cc
  tests/data-types/hash-tests.cc
  tests/data-types/ptrarray-tests.cc
  tests/data-types/array-tests.cc
  tests/data-types/list-tests.cc
  tests/render_tests.cc

  tests/data-types/pool-tests.cc)



set (DANGER_DEMO_SRC
  demos/exec.c

  demos/demo_2.c  demos/demo_2.h)

#--------------------------------------------
#-------------project  config-----------------
#--------------------------------------------

include_directories (
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/extern/kazmath

  ${CMAKE_SOURCE_DIR}/extern/nuklear

  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include)


#copy  resources  to  build
file (COPY  ${CMAKE_SOURCE_DIR}/resources/
  DESTINATION  ${CMAKE_BINARY_DIR}/resources)


file (GLOB  DANGER_RES  ${CMAKE_SOURCE_DIR}
  resources/*
  resources/**/*
  CMakeLists.txt
  em.js.*
  *.cmake)

add_custom_target (sls_resources
  COMMAND
  ${CMAKE_COMMAND}  -E  copy_directory
  ${CMAKE_SOURCE_DIR}/resources  ${CMAKE_BINARY_DIR}/  &&
  ${CMAKE_COMMAND}  -E  cmake_echo_color  --green
  "--  copying  resources/  to  ${CMAKE_BINARY_DIR}"
  COMMENT  "application  resources"
  SOURCES  ${DANGER_RES})

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -std=gnu++1y")
set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}  -std=gnu11")


if  ("${CMAKE_CXX_COMPILER_ID}"  STREQUAL  "Clang")
  set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}  -fms-extensions  -fblocks")
endif  ("${CMAKE_CXX_COMPILER_ID}"  STREQUAL  "Clang")

add_subdirectory (extern/googletest)
add_subdirectory (extern/Chipmunk2D)
add_subdirectory (extern/kazmath)
add_subdirectory (src/data-types)

#kazmath
set (KAZMATH_BUILD_JNI_WRAPPER  OFF  FORCE)
set (KAZMATH_BUILD_TESTS  OFF  FORCE)

#chipmunk
set (BUILD_DEMOS  OFF  FORCE)




set (CMAKE_LIBRARY_PATH
  "${CMAKE_LIBRARY_PATH}  \
  ${SDL2_LIBRARY_DIRS}  \
  ${SDL2IMG_LIBRARY_DIRS}"
  CACHE  STRING  "library  search  path")


#only  compile  gtest  and  glfw  when  not  using  emscripten

set (CMAKE_LINK_FLAGS  "${CMAKE_LINK_FLAGS}  ${SDL2_LDFLAGS}")
set (DANGER_DEPS
  ${DANGER_PTHREAD_LIB}
  kazmath
  gtest
  gmock
  ${DANGERTYPES_LIB}
  ${GLEW_LIBRARY}
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${OPENGL_gl_LIBRARY})



include_directories (
  ${GLEW_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${APR_INCLUDE_DIRS}
  ${APR_UTIL_INCLUDE_DIRS}
  extern/uthash/src
)

get_property (dirs  DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}  PROPERTY  INCLUDE_DIRECTORIES)

foreach  (dir  ${dirs})
  message (STATUS  "dir='${dir}'")
endforeach  ()


#---------------------------------------
#  build  dangerengine
#---------------------------------------

set (BUILD_SHARED_LIBS  ON)
add_library (dangerengine  STATIC  ${DANGER_SRC})

add_dependencies (dangerengine
  kazmath)

target_link_libraries (dangerengine
  ${DANGER_DEPS})

get_target_property (DANGERENGINE_OUTPUT  dangerengine  LIBRARY_OUTPUT_NAME)


#  copy  resources  on  build

add_custom_command (
  TARGET  sls_resources  PRE_BUILD
  COMMAND
  ${CMAKE_COMMAND}  -E  copy_directory
  ${CMAKE_SOURCE_DIR}/resources  ${CMAKE_BINARY_DIR}/  &&
  ${CMAKE_COMMAND}  -E  cmake_echo_color  --green
  "--  copying  resources/  to  ${CMAKE_BINARY_DIR}"
  COMMENT  "")

add_executable (demo
  ${DANGER_DEMO_SRC})


add_dependencies (demo
  dangerengine
  sls_resources)

target_link_libraries (demo
  dangerengine
  ${DANGER_DEPS})

#  add  a  separate  library  for  tests  compiled  in  C.  Used  for  supporting  build
#  systems  which  don't  compile  C  and  C++  code  separately

add_executable (tests  ${DANGER_TEST_SRC})
add_dependencies (tests
  gtest
  gtest_main
  dangerengine)
target_link_libraries (tests
  dangerengine
  ${DANGER_DEPS}
  gtest
  gtest_main)


if  (DOXYGEN_FOUND)

  set (DANGER_DOC_FILES
  "${CMAKE_SOURCE_DIR}/src/  \
  ${CMAKE_SOURCE_DIR}/src/data-types/  \
  ${CMAKE_SOURCE_DIR}/src/math/  \
  ${CMAKE_SOURCE_DIR}/extern/glfw/include/GLFW  \
  ${CMAKE_SOURCE_DIR}/extern/kazmath/kazmath  \
  ${CMAKE_SOURCE_DIR}/extern/kazmath/kazmath/GL")


  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile  @ONLY)

  add_custom_target (doc
  ${DOXYGEN_EXECUTABLE}  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT  "Generating  API  documentation  with  Doxygen"  VERBATIM)
  add_custom_target (view-html-docs
  open  ./html/index.html
  WORKING_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT  "opening  html  docs")

  add_dependencies (view-html-docs  doc)


endif  (DOXYGEN_FOUND)
