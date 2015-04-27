//
// Created by Steven on 4/25/15.
//

#ifndef DANGERENGINE_SLSUTILS_H
#define DANGERENGINE_SLSUTILS_H

#include <stdlib.h>

#define sls_msg(obj, method, ...) do { \
    typeof(obj) _obj_ = (obj);\
    _obj_->method(obj, ##__VA_ARGS__);\
} while(0)

void *sls_objalloc(void const *prototype, size_t size);

#endif //DANGERENGINE_SLSUTILS_H
