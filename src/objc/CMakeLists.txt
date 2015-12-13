cmake_minimum_required(VERSION 2.8)

option(SLS_USE_OBJC "compile with objective-c")

set(SLS_OBJC_SRC
    SLSContext.h
    SLSContext.m
    SLSAppState.h
    SLSAppState.m
    DangerEngineFW.h)

include_directories(
  ${CMAKE_SOURCE_DIR}src
  ${CMAKE_SOURCE_DIR}extern/kazmath

  ${CMAKE_SOURCE_DIR}extern/Chipmunk2D/include

  ${CMAKE_SOURCE_DIR}extern/googlemock/include
  ${CMAKE_SOURCE_DIR}extern/gtest/include
  ${APR_UTIL_INCLUDE_DIRS}
  /usr/local/include/
  /usr/local/Cellar/apr/1.5.1/libexec/include/apr-1
  /usr/local/Cellar/apr-util/1.5.4/include/apr-1)



if (SLS_USE_OBJC)
  set(SLS_OBJC_FLAGS "\
      -fobjc-arc   \
      -fmodules \
      -fexceptions -fobjc-exceptions \
      -funwind-tables -fblocks")
  
  set(CMAKE_C_FLAGS 
      "-ObjC ${SLS_OBJC_FLAGS} ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS 
      "-ObjC++ ${SLS_OBJC_FLAGS} ${CMAKE_CXX_FLAGS}")

  find_library(OBJFW objfw)
  find_library(OBJC objc)

  add_library(dangerengine-fw
      ${SLS_OBJC_SRC})

  add_dependencies(dangerengine-fw
      dangerengine)

  target_link_libraries(
      dangerengine-fw 
      dangerengine
      ${OBJFW}
      ${OBJC} )

endif(SLS_USE_OBJC)

