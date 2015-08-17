cmake_minimum_required(VERSION 2.8)

option(SLS_USE_OBJC "compile with objective-c")

set(SLS_OBJC_SRC
    SLSContext.h
    SLSContext.m
    DangerEngineFW.h)


if (SLS_USE_OBJC)
    set(SLS_OBJC_FLAGS
        "-fobjc-arc   \
        -fexceptions -fobjc-exceptions \
        -funwind-tables -fconstant-string-class=OFConstantString \
        -fno-constant-cfstrings -fblocks")
    
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

