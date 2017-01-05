/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 *
 **/
#include "geometries.h"

slsMesh *parametric_sphere_mesh_complex(size_t n_steps, int normal_dir,
                                        kmVec4 base_color);

slsMesh *sls_interior_sphere_mesh(size_t n_steps)
{
  return parametric_sphere_mesh_complex(n_steps, -1,
                                        (kmVec4){ 1.0, 1.0, 1.0, 1.0 });
}

slsMesh *sls_parametric_sphere_mesh(size_t n_steps)
{
  return parametric_sphere_mesh_complex(n_steps, 1,
                                        (kmVec4){ 1.0, 1.0, 1.0, 1.0 });
}

slsMesh *parametric_sphere_mesh_complex(size_t n_steps, int normal_dir,
                                        kmVec4 base_color)
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

    double theta = j * M_PI / ((double)n_latitudes);
    double sin_theta = sin(theta);
    double cos_theta = cos(theta);

    for (int i = 0; i <= n_longitudes; ++i) {

      double phi = i * 2 * M_PI / ((double)n_longitudes);
      double sin_phi = sin(phi);
      double cos_phi = cos(phi);

      assert((n_vertices) < _n_verts_alloced);

      kmVec3 normal = { cos_phi * sin_theta, cos_theta, sin_phi * sin_theta };

      kmVec3Scale(&normal, &normal, normal_dir);

      slsVec2 uv = {
        (i / (float)n_longitudes),
        (j / (float)n_latitudes) // png loads upside-down
      };

      kmVec3Normalize(&normal, &normal);

      kmVec3 position = {
        radius * normal.x, radius * normal.y, radius * normal.z,
      };

      assert(!(sls_nearf(position.x, 0.0, 0.01) &&
               sls_nearf(position.y, 0.0, 0.01) &&
               sls_nearf(position.z, 0.0, 0.01)));

      // sls_log_info("#%i: %f %f %f", n_vertices,
      //             position.x, position.y, position.z);

      memcpy(verts[n_vertices].position, &position, sizeof(float) * 3);

      memcpy(verts[n_vertices].color, (float *)&base_color, sizeof(float) * 4);
      memcpy(verts[n_vertices].normal, &normal, sizeof(float) * 3);
      memcpy(verts[n_vertices].uv, &uv, sizeof(float) * 2);

      idxs[n_vertices] = (uint32_t)n_vertices;
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

      uint32_t quad[] = { first,  second,     first + 1,
                          second, second + 1, first + 1 };

      for (int i = 0; i < sizeof(quad) / sizeof(uint32_t); ++i) {
        if (quad[i] > n_vertices - 2) {
          uint32_t qx = (uint32_t)(quad[i] % n_longitudes);
          uint32_t qy = (uint32_t)(quad - qx % n_latitudes);
          qx = 0;

          quad[i] = (uint32_t)(qy * (n_longitudes) + qx);
        }
      }

      // sls_log_info("%u %u %u %u %u %u",
      //             quad[0], quad[1], quad[2], quad[3], quad[4], quad[5]);

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

double sls_sphere_radius_at_height(double radius, double height)
{
  return sqrt((radius * radius) - height * height);
}

slsMesh *sls_cube_mesh(bool use_cubemap_texcoords)
{
  slsVertex verts[] = { //
                        // front
                        {.position = { -1.f, -1.f, 1.f },
                         .normal = { 0.0, 0.0, 1.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { 1.f, -1.f, 1.f },
                         .normal = { 0.0, 0.0, 1.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { 1.f, 1.f, 1.f },
                         .normal = { 0.0, 0.0, 1.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { -1.f, 1.f, 1.f },
                         .normal = { 0.0, 0.0, 1.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } },

                        // back
                        {.position = { -1.f, -1.f, -1.f },
                         .normal = { 0.0, 0.0, -1.f },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { 1.f, -1.f, -1.f },
                         .normal = { 0.0, 0.0, -1.f },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { 1.f, 1.f, -1.f },
                         .normal = { 0.0, 0.0, -1.f },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { -1.f, 1.f, -1.f },
                         .normal = { 0.0, 0.0, -1.f },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } },

                        // top
                        {.position = { -1.f, 1.f, -1.f },
                         .normal = { 0.0, 1.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { 1.f, 1.f, -1.f },
                         .normal = { 0.0, 1.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { 1.f, 1.f, 1.f },
                         .normal = { 0.0, 1.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { -1.f, 1.f, 1.f },
                         .normal = { 0.0, 1.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } },

                        // bottom
                        {.position = { -1.f, -1.f, -1.f },
                         .normal = { 0.0, -1.f, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { 1.f, -1.f, -1.f },
                         .normal = { 0.0, -1.f, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { 1.f, -1.f, 1.f },
                         .normal = { 0.0, -1.f, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { -1.f, -1.f, 1.f },
                         .normal = { 0.0, -1.f, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } },

                        // right
                        {.position = { 1.f, -1.f, -1.f },
                         .normal = { 1.0, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { 1.f, 1.f, -1.f },
                         .normal = { 1.0, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { 1.f, 1.f, 1.f },
                         .normal = { 1.0, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { 1.f, -1.f, 1.f },
                         .normal = { 1.0, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } },

                        // left
                        {.position = { -1.f, -1.f, -1.f },
                         .normal = { -1.f, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 0.f } },
                        {.position = { -1.f, 1.f, -1.f },
                         .normal = { -1.f, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 0.f } },
                        {.position = { -1.f, 1.f, 1.f },
                         .normal = { -1.f, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 1.f, 1.f } },
                        {.position = { -1.f, -1.f, 1.f },
                         .normal = { -1.f, 0.0, 0.0 },
                         .color = { 1.f, 1.f, 1.f, 1.f },
                         .uv = { 0.f, 1.f } }
  };
  uint32_t indices[] = {
    0, 1, 2, 0, 2, 3, 2, 1, 0, 3, 2, 0,

    0, 1, 2, 0, 2, 3, 2, 1, 0, 3, 2, 0,

    0, 1, 2, 0, 2, 3, 2, 1, 0, 3, 2, 0,

  };

  for (int j = 0; j < 6; ++j) {
    uint32_t *iter = indices + j * 6;
    for (int i = 0; i < 6; ++i) {
      iter[i] += j * 4;
      printf("%u ", iter[i]);
    }
    puts("\n");
  }

  size_t n_verts = sizeof(verts) / sizeof(*verts);
  size_t n_indices = sizeof(indices) / sizeof(*indices);

  return sls_mesh_new(verts, n_verts, indices, n_indices);
}
