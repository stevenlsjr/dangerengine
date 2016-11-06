/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/2/16, Steven
 * 
 **/
#include "slstexture.h"
#include <SDL2/SDL_image.h>

GLuint sls_image_load(char const *path)
{
  SDL_Surface *img = NULL;
  GLuint tex_id = 0;

  img = IMG_Load(path);
  sls_checkmem(img);

  GLenum mode;
  SDL_PixelFormat const *fmt = img->format;

  switch (fmt->BytesPerPixel) {
    case 3:
      mode = GL_RGB;
      break;
    case 4:
      mode = GL_RGBA;
      break;
    default:
      sls_check(0, "invalid image format bpp: %u", img->format->BitsPerPixel);
  }



  glGenTextures(1, &tex_id);

  // load image to texture
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, mode, img->w, img->h, 0,
               mode,
               GL_UNSIGNED_BYTE,
               img->pixels);

  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  SDL_FreeSurface(img);

  return tex_id;

error:
  if (img) {
    SDL_FreeSurface(img);
  }

  if (glIsTexture(tex_id)) {
    glDeleteTextures(1, &tex_id);
  }
  return 0;
}
