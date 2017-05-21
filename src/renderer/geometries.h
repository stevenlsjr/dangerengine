/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#ifndef DANGERENGINE_SPHERE_H
#define DANGERENGINE_SPHERE_H

#include <dangerengine.h>

slsMesh* sls_parametric_sphere_mesh(size_t n_steps);

slsMesh* sls_interior_sphere_mesh(size_t n_steps);

slsMesh* sls_cube_mesh(bool use_cubemap_texcoords);

double sls_sphere_radius_at_height(double radius, double height);

#endif // DANGERENGINE_SPHERE_H
