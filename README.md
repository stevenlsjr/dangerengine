dangerengine
==============
OpenGL engine in C


**Goals**:

* Create a demonstration rendering engine in C
* Practice object-oriented design in C
* Demonstrate modern OpenGL development
* Possibly implement browser deployment via emscripten

## to build
* `$ git submodule update --init`
* create separate build directory & change to that directory
* `$ cmake $(PATH_TO_ROOT_SOURCE_DIR)`
* `$ make`
* run executables `demo` or `tests`


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


