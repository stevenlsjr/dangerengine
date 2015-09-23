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
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea.
**/


#include "slstexture.h"

#include <assert.h>

/*----------------------------*
 * prototypes
 *----------------------------*/

/**
 * @brief Private texture information
 * @detail
 **/
struct slsTexture_p {
  GLuint program;

};


/*----------------------------*
 * prototypes
 *----------------------------*/

slsTexture *sls_texture_init(slsTexture *self,
                             char const *diffuse_path,
                             char const *specular_path,
                             char const *normal_path);

void sls_texture_dtor(slsTexture *self);

void sls_texture_set_program(slsTexture *self, GLuint program);


GLuint sls_texture_get_program(slsTexture *self);

void sls_texture_bind(slsTexture *self);

/*----------------------------*
 * class implementation
 *----------------------------*/

slsTexture const *sls_texture_class()
{

  static slsTexture proto = {
      .init         =sls_texture_init,
      .dtor         =sls_texture_dtor,
      .set_program  = sls_texture_set_program,
      .get_program  = sls_texture_get_program,
      .bind         =sls_texture_bind
  };

#pragma clang diagnostic push
  // suppress warning for returning address to static memory
#pragma clang diagnostic ignored "-Wreturn-stack-address"
  return &proto;
#pragma clang diagnostic pop
}

slsTexture *sls_texture_new(char const *diffuse_path,
                            char const *specular_path,
                            char const *normal_path)
{
  slsTexture *self = sls_objalloc(sls_texture_class(), sizeof(slsTexture));

  return sls_msg(self, init, diffuse_path, specular_path, normal_path);
}

slsTexture *sls_texture_init(slsTexture *self,
                             char const *diffuse_path,
                             char const *specular_path,
                             char const *normal_path)
{
  if (!self) { return NULL; }

  self->priv = calloc(1, sizeof(slsTexture_p));
  sls_checkmem(self->priv);

  const int width = -1;
  const int height = -1;
  // a simple tuple struct for iterating 
  struct tmp_tuple {
    char const *path;
    slsTexPair *pair;
  };

  struct tmp_tuple tups[] = {
      {diffuse_path,  &self->diffuse},
      {specular_path, &self->specular},
      {normal_path,   &self->normal},
  };

  for (int i = 0;
       i < sizeof(tups) / sizeof(struct tmp_tuple);
       ++i) {

    struct tmp_tuple const tup = tups[i];
    tup.pair->is_bound = SLS_FALSE;

    if (tup.path) {
      tup.pair->unit = sls_gltex_from_file(tup.path,
                                           width,
                                            height);

      tup.pair->is_active = SLS_TRUE;

    } else {
      tup.pair->is_active = SLS_FALSE;
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

void sls_texture_dtor(slsTexture *self)
{
  if (!self) { return; }

  if (self->priv) {
    free(self->priv);
  }

  free(self);
}

void sls_texture_set_program(slsTexture *self, GLuint program)
{
  if (!self) { return; }
  sls_check(glIsProgram(program), "object %ui is not a program handle", program);

  int diffuse, specular, normal;
  diffuse = glGetUniformLocation(program, "diffuse_map");
  specular = glGetUniformLocation(program, "specular_map");
  normal = glGetUniformLocation(program, "normal_map");

  if (diffuse < 0 && self->diffuse.is_active) {
    sls_log_warn("could not find `diffuse_map` location in shader program");
  } else {
    self->diffuse.uniform = (GLuint) diffuse;
  }

  if (specular < 0 && self->specular.is_active) {
    sls_log_warn("could not find `specular_map` location in shader program");
  } else {
    self->specular.uniform = (GLuint) specular;
  }

  if (normal < 0 && self->normal.is_active) {
    sls_log_warn("could not find `normal_map` location in shader program");
  } else {
    self->normal.uniform = (GLuint) normal;
  }

  self->priv->program = program;
  return;
  error:
  return;
}


GLuint sls_texture_get_program(slsTexture *self)
{
  assert(self);
  assert(self->priv);

  return self->priv->program;
}

void sls_texture_bind(slsTexture *self)
{
  if (!self) { return; }
  glUseProgram(self->priv->program);

  slsTexPair *pairs[] = {&self->diffuse,
                         &self->specular,
                         &self->normal};

  for (size_t i = 0; i < sizeof(pairs) / sizeof(slsTexPair); ++i) {
    slsTexPair *p = pairs[i];
    if (p && p->is_active) {
      glUniform1i(p->uniform, (GLint) i);
      glBindTexture(GL_TEXTURE_2D, p->unit);
      p->is_bound = SLS_TRUE;
    }
  }
}


/*----------------------------*
 * utility functions
 *----------------------------*/

#ifndef __EMSCRIPTEN__
GLuint sls_gltex_from_file(char const *path,
                           int width_opt,
                           int height_opt)
{

  ILuint img;
  GLuint tex;
  int width, height, depth, bpp;
  ILenum il_format, il_type;
  GLenum gl_format, gl_type;


  gl_format = GL_RGB;

  il_type = IL_UNSIGNED_BYTE;
  gl_type = GL_UNSIGNED_BYTE;


  ilGenImages(1, &img);
  glGenTextures(1, &tex);


  ilBindImage(img); // bind handler to il

  sls_il_get_errors(); // pop errors

  if (!ilLoadImage(path)) {
    sls_il_get_errors();
    sls_log_err("image load failed!");
    return tex;
  }


  depth = ilGetInteger(IL_IMAGE_DEPTH);
  bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
  il_format = ilGetInteger(IL_IMAGE_FORMAT);

  if (il_format != IL_RGBA && il_format != IL_RGB) {
    sls_log_warn("il format unexpected");
  }

  gl_format = (il_format == IL_RGBA) ? GL_RGBA : GL_RGB;


  width = (width_opt > 0) ?
          width_opt :
          ilGetInteger(IL_IMAGE_WIDTH);
  height = (height_opt > 0) ?
           height_opt :
           ilGetInteger(IL_IMAGE_HEIGHT);


  // format image to specifications
  ilTexImage(width, height, depth, bpp, il_format, il_type, NULL);


  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D,
               0, /* level */
               gl_format, /* internalformat */
               width, height,
               0, /* border=0 */
               gl_format, gl_type, /* format, type */
               ilGetData() /* data */);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  ilDeleteImage(img);

  return tex;
}

#else
GLuint sls_gltex_from_file(char const *path,
                           int width_opt,
                           int height_opt)
{
  return 0;
}
#endif

#ifndef __EMSCRIPTEN__

ILenum sls_il_get_errors()
{
  ILenum last;
  last = IL_NO_ERROR;
  for (ILenum err = ilGetError();
       err != IL_NO_ERROR;
       err = ilGetError()) {
    
    size_t max_cmp_size = 1000;
    char const *err_str =ilGetString(err);
    if (!err_str || strncasecmp(err_str, "(null)", max_cmp_size) == 0) {
      break;
    }
    
    
    sls_log_warn("devIL: %s", err_str);
    if (err == IL_OUT_OF_MEMORY) {
      sls_log_fatal("out of memory error!");
    }

    if (last == IL_NO_ERROR) {
      last = err;
    }
  }

  return last;
}

#endif
