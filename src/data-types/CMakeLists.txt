cmake_minimum_required(VERSION 2.8)

# library wrapper for data types

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/extern/stb)

set (DANGERTYPES_SRC
     dangertypes.h
     array.c array.h
     callbacks.c callbacks.h
     hashtable.c hashtable.h
     intrusivelist.h
     linkedlist.c linkedlist.h
     ptrarray.c ptrarray.h
     slsPool.c slsPool.h
     slsPool_p.h)



set (DANGERTYPES_LIB dangertypes
     CACHE STRING "dangerengine's data types module")
add_library(${DANGERTYPES_LIB} ${DANGERTYPES_SRC})
