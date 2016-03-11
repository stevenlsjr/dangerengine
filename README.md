dangerengine
==============
OpenGL engine in C


**Goals**:

* Create a demonstration rendering engine in C
* Practice object-oriented design in C
* Demonstrate modern OpenGL development
* Possibly implement browser deployment via emscripten

## to build
2) create separate build directory
3) `$ cmake $(PATH_TO_ROOT_SOURCE_DIR)`
4) `$ make`
5) run executables `demo` or `tests`


## note:
this branch is undergoing major refactoring and
is not functional now

## Project dependencies

* SDL2 (including SDL2_IMG) (not included)
* gtest (included as git submodule)
* kazmath (included as git submodule)
* chipmunk 2d (included as git submodule)
* stb (included as a git submodule)
* glew (not included)


