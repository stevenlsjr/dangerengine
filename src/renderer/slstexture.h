/**
 * @file ${FILE}
 * @brief
 * @license ${LICENSE}
 * Copyright (c) 10/2/16, Steven
 *
 **/
#ifndef DANGERENGINE_SLSTEXTURE_H
#define DANGERENGINE_SLSTEXTURE_H

#include <sls-gl.h>

typedef struct slsTexture slsTexture;
/**
 * @brief Stores and represents a GPU texture
 * instance
 */
struct slsTexture {
  GLuint texture;
};

GLuint sls_image_load(char const *path) SLS_NONNULL(1);

#endif // DANGERENGINE_SLSTEXTURE_H
