/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/17/15, Steven
 * 
 **/
#include "tile_map.h"
#include <apr-1/apr_strings.h>
#include <state/slsEntity.h>


void sls_tilemap_create_children(slsEntity *self, apr_pool_t *pool);

slsEntity *sls_create_tilemap(slsAppState *state,
                              char const *name, kmVec2 position, float direction,
                              slsTilemapData const *data)
{
  slsEntity *tilemap = NULL;
  size_t map_size = data->width * data->height;

  tilemap = calloc(1, sizeof(slsEntity));

  sls_checkmem(sls_entity_init(tilemap, state->context->pool, SLS_COMPONENT_NONE, name));

  slsTilemapData *dup_data = apr_pcalloc(tilemap->pool, sizeof(slsTilemapData));
  tilemap->behavior.data = dup_data;

  *dup_data = *data;
  sls_check(map_size > 0, "invalid map size %lu %lu", data->width, data->height);

  dup_data->map = apr_pcalloc(tilemap->pool, sizeof(slsTileVals) * map_size + 1);
  memcpy(dup_data->map, data->map, sizeof(slsTileVals) * map_size);


  assert(dup_data->tile_shader && dup_data->tile_texture);


  sls_tilemap_create_children(tilemap, state->context->pool);

  sls_transform2d_init(&tilemap->transform);
  tilemap->transform.pos = position;
  tilemap->transform.rot = direction;

  return tilemap;

  error:
  if (tilemap) {
    sls_entity_delete(tilemap);
  }
  return NULL;

}


void sls_tilemap_create_children(slsEntity *self, apr_pool_t *pool)
{
  slsTilemapData *data = self->behavior.data;
  size_t n_children = data->width * data->height;

  data->child_sprites = apr_pcalloc(pool, n_children * sizeof(slsEntity) * (1 + n_children * 2));
  assert(data->tile_shader && data->tile_texture);

  kmVec2 tile_offset = {2.0, 2.0};

  apr_pool_t *tmp_pool = NULL;
  sls_checkmem(
      apr_pool_create(&tmp_pool, self->pool) == APR_SUCCESS);


  for (int j = 0; j < data->height; ++j) {
    for (int i = 0; i < data->width; ++i) {
      size_t true_idx = j * data->width + i;

      printf("%i", data->map[true_idx]);
      slsEntity *child = data->child_sprites + true_idx;
      size_t map_size = data->height * data->width;


      child->transform.scale = (kmVec2){0.5, 0.5};

      slsTexture *tex = data->tile_texture;

      char *_name = NULL;
      _name = apr_psprintf(tmp_pool, "tile,%i,%i", i, j);
      sls_init_sprite(child, pool, _name, tex, data->tile_shader);

      child->transform.pos.x = tile_offset.x * i;
      child->transform.pos.y = tile_offset.y * j;
      child->transform.z_layer += 2;

      sls_entity_addchild(self, child);


      if (data->map[true_idx] == SLS_TILE_OBST) {
        slsEntity *obstacle = data->child_sprites + map_size + true_idx;
        char *o_name = apr_psprintf(tmp_pool, "%s,obstacle", _name);

        sls_entity_addchild(child, sls_init_sprite(obstacle,
                                                   pool, o_name, data->block_texture, data->tile_shader));
        obstacle->transform.z_layer = -5;

        obstacle->component_mask &= ~SLS_COMPONENT_KINETIC;

      }

      child->component_mask &= ~SLS_COMPONENT_KINETIC & ~SLS_COMPONENT_BOUNDED;


      child->material_is_owned = false;
      child->mesh_is_owned = true;

    }
    printf("\n");
  }

  apr_pool_destroy(tmp_pool);
  return;
  error:
  assert(0);
  return;
}
