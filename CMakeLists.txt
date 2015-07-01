cmake_minimum_required(VERSION 2.8)

project(dangerengine)

file(COPY ${CMAKE_SOURCE_DIR}/resources/
    DESTINATION ${CMAKE_BINARY_DIR}/resources)

file(GLOB DANGER_RES ${CMAKE_SOURCE_DIR}
  resources/*
  resources/**/*
  CMakeLists.txt
  em.js.*
  *.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

add_subdirectory(extern/kazmath)
set (KAZMATH_BUILD_TESTS OFF)
set (KAZMATH_BUILD_JNI_WRAPPER OFF)
set (KAZMATH_BUILD_GL_UTILS OFF)

set(DANGER_DEPS kazmath)

find_package(OpenGL)
find_package(GLEW)
find_package(Doxygen)
find_package(DevIL)

if (NOT EMSCRIPTEN)
  add_subdirectory(extern/googlemock)
  add_subdirectory(extern/glfw)

  set(DANGER_DEPS
    ${DANGER_DEPS}
    # glfw
    glfw
    ${GLFW_LIBRARIES}
    ${DEVIL_LIBRARIES}
    ${OPENGL_gl_LIBRARY})


  include_directories(
    ${IL_INCLUDE_DIR})

  if (GLEW_FOUND)
    set (DANGER_DEPS
      ${DANGER_DEPS}
      ${GLEW_LIBRARIES})
    include_directories(
      ${GLEW_INCLUDE_DIR}
      ${GLFW_SOURCE_DIR}/include
      ${OPENGL_INCLUDE_DIR})
  endif(GLEW_FOUND)



else (NOT EMSCRIPTEN) # if emscripten
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g4")
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} --pre-js ${CMAKE_SOURCE_DIR}/em.js.pre")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} --post-js ${CMAKE_SOURCE_DIR}/em.js.post")

  set(CMAKE_C_DEBUG_FLAGS "${CMAKE_C_FLAGS} -g4 -Wno-warn-absolute-paths --emit-symbol-map")
endif (NOT EMSCRIPTEN)


# included files listing sources
include(engine_src.cmake)
include(test_src.cmake)

include_directories(
    ${CMAKE_SOURCE_DIR}/extern/kazmath
    ${CMAKE_SOURCE_DIR}/extern/googlemock/include
    ${CMAKE_SOURCE_DIR}/extern/gtest/include)

add_library(dangerengine ${DANGER_SRC} ${DANGER_RES})
target_link_libraries(dangerengine
    ${DANGER_DEPS})


add_executable(dangerbird_bin 
    src/exec.c)
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


endif(DOXYGEN_FOUND)
