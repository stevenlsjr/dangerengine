/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/17/15, Steven
 * 
 **/
#ifndef DANGERENGINE_TILE_MAP_H
#define DANGERENGINE_TILE_MAP_H

#include <stdbool.h>
#include <dangerengine.h>

typedef enum slsTileVals {
  SLS_TILE_GROUND = 0,
  SLS_TILE_OBST = 1
} slsTileVals;

typedef struct slsTilemapData {
  slsVRect sprite_a;
  slsVRect sprite_b;
  size_t width;
  size_t height;

  int *map;
  slsEntity *child_sprites;

  slsMaterial *tile_texture;
  slsMaterial *block_texture;
  slsShader *tile_shader;

} slsTilemapData;



slsEntity *sls_create_tilemap(slsAppState *state, char const *name, kmVec2 position, float direction,
                              slsTilemapData const *data) SLS_NONNULL(1, 2, 5);

int sls_tilemap_val_at_point(slsEntity *tilemap, kmVec2 *point);


#endif //DANGERENGINE_TILE_MAP_H
