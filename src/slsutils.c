//
// Created by Steven on 4/25/15.
//

#include "slsutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *sls_objalloc(void const *prototype, size_t size) {
    void *obj = NULL;
    obj = calloc(size, 1);
    if (!obj) {
        fprintf(stderr, "memory error!\n");
        return NULL;
    }
    memcpy(obj, prototype, size);
    return obj;
}
