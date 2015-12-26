/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SPHERE_H
#define DANGERENGINE_SPHERE_H

#include <tgmath.h>
#include <dangerengine.h>


slsMesh * parametric_sphere_mesh(size_t n_steps);

double sphere_radius_at_height(double radius, double height);


#endif //DANGERENGINE_SPHERE_H
