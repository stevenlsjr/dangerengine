//
// Created by Steven on 4/27/15.
//

#include <stdio.h>
#include "c_cases.h"
#include "../src/dangerengine.h"


#include <kazmath/kazmath.h>



bool sls_add_vec2() {
    bool res = false;
    kmVec2 a = {1, 2};
    kmVec2 b = {1, 2};


    kmVec2 c = sls_vec2_add(&a, &b);
    kmVec2 exp = {2, 4};
    res = (bool)kmVec2AreEqual(&c, &exp);
    if (!res) {
        fprintf(stderr, "c: {%f %f} should equal {%f %f}\n", c.x, c.y, exp.x, exp.y);
    }

    return res;
}

bool sls_add_vec3()
{
    bool res = false;

    kmVec3 a = {2, 3, 4};
    kmVec3 b = {2, 3, 4};

    kmVec3 exp = {2, 3, 4};
    kmVec3 c;
    c = sls_vec3_add(&a, &b);


    res = (bool)kmVec3AreEqual(&c, &exp);
    if (!res) {
        fprintf(stderr, "%s failed\n", __func__);
    }

    return res;
}

bool sls_test_addmacro() {

    return sls_add_vec2() && sls_add_vec3();
}