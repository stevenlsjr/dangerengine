/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 * 
 **/
#include <cppapi.h>
#include "sphere.h"

slsMesh *parametric_sphere_mesh(size_t n_steps)
{
  slsMesh *mesh = NULL;

  size_t _n_verts_alloced = n_steps * n_steps + 16 + 2 * n_steps;

  size_t _n_indices_alloced = _n_verts_alloced * 6 + 3;

  size_t n_latitudes = n_steps;
  size_t n_longitudes = n_steps;


  slsVertex *verts = calloc(_n_verts_alloced, sizeof(slsVertex));
  uint32_t *idxs = calloc(_n_indices_alloced, sizeof(uint32_t));


  float radius = 1.f;


  size_t n_vertices = 0;

  for (int j = 0; j <= n_latitudes; ++j) {

    double theta = j * M_PI / ((double) n_latitudes);
    double sin_theta = sin(theta);
    double cos_theta = cos(theta);

    for (int i = 0; i <= n_longitudes; ++i) {

      double phi = i * 2 * M_PI / ((double) n_longitudes);
      double sin_phi = sin(phi);
      double cos_phi = cos(phi);

      assert((n_vertices) < _n_verts_alloced);


      kmVec3 normal = {
          cos_phi * sin_theta,
          cos_theta,
          sin_phi * sin_theta
      };

      slsVec2 uv = {
          (i / (float) n_longitudes),
          (j / (float) n_latitudes) // png loads upside-down
      };

      kmVec3Normalize(&normal, &normal);

      kmVec3 position = {
          radius * normal.x,
          radius * normal.y,
          radius * normal.z,
      };

      assert(!(sls_nearf(position.x, 0.0, 0.01) &&
               sls_nearf(position.y, 0.0, 0.01) &&
               sls_nearf(position.z, 0.0, 0.01)));

      sls_log_info("#%i: %f %f %f", n_vertices,
                   position.x, position.y, position.z);


      memcpy(verts[n_vertices].position, &position, sizeof(float) * 3);

      memcpy(verts[n_vertices].color, ((float[4]) {1.0, 1.0, 1.0, 1.0}), sizeof(float) * 4);
      memcpy(verts[n_vertices].normal, &normal, sizeof(float) * 3);
      memcpy(verts[n_vertices].uv, &uv, sizeof(float) * 2);


      idxs[n_vertices] = (uint32_t) n_vertices;
      n_vertices++;

    }

  }

  uint32_t *index_itor = idxs;

  size_t n_indices = 0;

  for (int j_lat = 0; j_lat < n_latitudes; ++j_lat) {
    for (int i_long = 0; i_long < n_longitudes; ++i_long) {

      uint32_t first = (j_lat * (n_longitudes + 1)) + i_long;
      uint32_t second = (first + n_longitudes + 1);


      const size_t n = 6;

      uint32_t quad[] = {
          first,
          second,
          first + 1,
          second,
          second + 1,
          first + 1
      };

      for (int i = 0; i < sizeof(quad) / sizeof(uint32_t); ++i) {
        if (quad[i] > n_vertices - 2) {
          uint32_t qx = quad[i] % n_longitudes;
          uint32_t qy = quad - qx % n_latitudes;
          qx = 0;

          quad[i] = (qy * (n_longitudes) + qx);

        }
      }


      sls_log_info("%u %u %u %u %u %u",
                   quad[0], quad[1], quad[2], quad[3], quad[4], quad[5]);

      memcpy(index_itor, quad, sizeof(uint32_t) * 6);


      index_itor += n;
      n_indices += n;

    }
  }


  mesh = sls_mesh_new(verts, n_vertices, idxs, n_indices);

  free(verts);

  free(idxs);

  return mesh;


}

double sphere_radius_at_height(double radius, double height)
{
  return sqrt((radius * radius) - height * height);
}