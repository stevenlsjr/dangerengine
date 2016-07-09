//
// Created by Steven on 7/1/15.
//

/**
 * @file slstexture.c
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#include "slsmaterial.h"

#include <assert.h>
#include <SDL2/SDL_image.h>

/*----------------------------*
 * prototypes
 *----------------------------*/

/**
 * @brief Private texture information
 * @detail
 **/
struct slsMaterial_p {
  GLuint program;
};

/*----------------------------*
 * prototypes
 *----------------------------*/

slsMaterial *sls_texture_init(slsMaterial *self, char const *diffuse_path,
                              char const *specular_path,
                              char const *normal_path);

void sls_texture_dtor(slsMaterial *self);

void sls_texture_set_program(slsMaterial *self, GLuint program);

GLuint sls_texture_get_program(slsMaterial *self);

void sls_texture_bind(slsMaterial *self);

/*----------------------------*
 * class implementation
 *----------------------------*/

slsMaterial const *sls_texture_class() {

  static slsMaterial proto = {.init = sls_texture_init,
                              .dtor = sls_texture_dtor,
                              .set_program = sls_texture_set_program,
                              .get_program = sls_texture_get_program,
                              .bind = sls_texture_bind};

#pragma clang diagnostic push
// suppress warning for returning address to static memory
#pragma clang diagnostic ignored "-Wreturn-stack-address"
  return &proto;
#pragma clang diagnostic pop
}

slsMaterial *sls_texture_new(char const *diffuse_path,
                             char const *specular_path,
                             char const *normal_path) {
  slsMaterial *self = sls_objalloc(sls_texture_class(), sizeof(slsMaterial));

  return sls_msg(self, init, diffuse_path, specular_path, normal_path);
}

slsMaterial *sls_texture_init(slsMaterial *self, char const *diffuse_path,
                              char const *specular_path,
                              char const *normal_path) {
  if (!self) {
    return NULL;
  }

  self->priv = calloc(1, sizeof(slsMaterial_p));
  sls_checkmem(self->priv);

  const int width = -1;
  const int height = -1;
  // a simple tuple struct for iterating
  struct tmp_tuple {
    char const *path;
    slsTexPair *pair;
  };

  struct tmp_tuple tups[] = {
      {diffuse_path, &self->diffuse},
      {specular_path, &self->specular},
      {normal_path, &self->normal},
  };

  for (int i = 0; i < sizeof(tups) / sizeof(struct tmp_tuple); ++i) {

    struct tmp_tuple const tup = tups[i];

    if (tup.path) {
      tup.pair->gltex = sls_gltex_from_file(tup.path, width, height);

      tup.pair->is_active = true;

    } else {
      tup.pair->is_active = false;
    }

    if (tup.path) {
      assert(tup.pair->is_active);
    }
  }

  return self;

error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
  return NULL;
}

void sls_texture_dtor(slsMaterial *self) {
  if (!self) {
    return;
  }

  if (self->priv) {
    free(self->priv);
  }

  free(self);
}

void sls_texture_set_program(slsMaterial *self, GLuint program) {
  if (!self) {
    return;
  }
  sls_check(glIsProgram(program), "object %ui is not a program handle",
            program);

  int diffuse, specular, normal;
  diffuse = glGetUniformLocation(program, "diffuse_tex");
  specular = glGetUniformLocation(program, "specular_tex");
  normal = glGetUniformLocation(program, "normal_tex");

  if (diffuse < 0 && self->diffuse.is_active) {
    sls_log_warn("could not find `diffuse_tex` location in shader program");
  } else {
    self->diffuse.uniform = (GLuint)diffuse;
  }

  if (specular < 0 && self->specular.is_active) {
    sls_log_warn("could not find `specular_tex` location in shader program");
  } else {
    self->specular.uniform = (GLuint)specular;
  }

  if (normal < 0 && self->normal.is_active) {
    sls_log_warn("could not find `normal_tex` location in shader program");
  } else {
    self->normal.uniform = (GLuint)normal;
  }

  self->priv->program = program;
  return;
error:
  return;
}

GLuint sls_texture_get_program(slsMaterial *self) {
  assert(self);
  assert(self->priv);

  return self->priv->program;
}

void sls_texture_bind(slsMaterial *self) {
  if (!self) {
    return;
  }
  glUseProgram(self->priv->program);

  slsTexPair pairs[] = {self->diffuse, self->specular, self->normal};

  GLenum textures[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};

  for (size_t i = 0; i < sizeof(pairs) / sizeof(slsTexPair); ++i) {
    slsTexPair *p = pairs + i;

    if (p && p->is_active) {
      glActiveTexture(textures[i]);

      //glUniform1i(p->uniform, (GLint)p->gltex);
      glBindTexture(GL_TEXTURE_2D, p->gltex);
    }
  }
}

/*----------------------------*
 * utility functions
 *----------------------------*/

GLuint sls_gltex_from_file(char const *path, int width_opt, int height_opt) {
  GLuint tex = 0;

  SDL_Surface *img = NULL;

  img = IMG_Load(path);
  sls_check(img, "file loading failed");

  tex = sls_gltex_from_surface(img);

  return tex;
error:

  sls_log_err("IMG: %s", IMG_GetError());

  if (img) {
    SDL_FreeSurface(img);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  return 0;
}

GLuint sls_gltex_from_surface(SDL_Surface *surface) {

  GLuint tex = 0;
  GLenum gl_format, gl_type;

  glGenTextures(1, &tex);

  gl_format = sls_glformat_from_sdlformat(surface->format);

  sls_check(gl_format != GL_FALSE, "invalid surface pixel format");

  gl_type = GL_UNSIGNED_BYTE;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  sls_check(surface->pixels, "no pixel vertex");

  sls_check(surface->w > 0 && surface->h > 0, "size %i %i invalid", surface->w,
            surface->h);

  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D,    // enum
               0,                // level
               gl_format,        // internalformat
               surface->w,       // width
               surface->h,       // height
               0,                // border = 0
               gl_format,        // format
               gl_type,          // data type
               surface->pixels); // data

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return tex;
error:
  assert(0);

  sls_log_err("IMG: %s", IMG_GetError());

  glBindTexture(GL_TEXTURE_2D, 0);
  return 0;
}

GLenum sls_glformat_from_sdlformat(SDL_PixelFormat const *fmt) {
  GLenum gl_format = GL_RGBA;

  int bpp = fmt->BytesPerPixel, rmask = fmt->Rmask;

  switch (bpp) {
  case 3: {
    gl_format = (rmask == 0x000000ff) ? GL_BGR : GL_RGB;
    break;
  }
  case 4: {
    gl_format = (rmask == 0x000000ff) ? GL_BGRA : GL_RGBA;
    break;
  }
  default:
    sls_log_warn("unsupported bytes per pixel: %i", bpp);

    gl_format = GL_FALSE;
    break;
  }

  return gl_format;
}
