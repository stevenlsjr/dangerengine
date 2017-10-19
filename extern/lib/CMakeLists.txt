# Steven Shea
# vendor sources subdirectory

set(VENDOR_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor.c ${CMAKE_CURRENT_SOURCE_DIR}/vendor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nuklear.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nuklear_sdl_gl3.h
    ${CMAKE_SOURCE_DIR}/extern/glad_debug/src/glad.c
    PARENT_SCOPE)