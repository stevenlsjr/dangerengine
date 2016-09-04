// dummyclang.cpp : Defines the exported functions for the static library.
//

#include "dummyclang.h"

// This is an example of an exported variable
int ndummyclang=0;

// This is an example of an exported function.
int fndummyclang(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see dummyclang.h for the class definition
Cdummyclang::Cdummyclang()
{
    return;
}
