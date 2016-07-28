//
// Created by Steven on 7/1/15.
//

/**
 * @file slstexture.h
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
 *    and/or other materials provided with the dist.
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

#ifndef DANGERENGINE_SLSTEXTURE_H
#define DANGERENGINE_SLSTEXTURE_H

#include "../sls-gl.h"
#include "../sls-imagelib.h"
#include "../slsutils.h"
#include <inttypes.h>
#include <kazmath/vec3.h>

typedef struct slsMaterial slsMaterial;
typedef struct slsMaterial_p slsMaterial_p;
typedef struct slsTexPair slsTexPair;

struct slsTexPair {
  /**
   * @brief texture object created by glGenTextures
   */
  GLuint gltex;

  /**
   * @brief uniform name of texture
   */
  GLuint uniform;
  /**
   * @brief flags the texture's successful load in memory
   */
  bool is_active;
};

struct slsMaterial {
  /**
   * @brief initializes a slsMaterial object.
   * @details all parameters (aside from `self`) are optional:
   * if a path parameter is NULL, the slsTexPair will be marked as
   * inactivek
   * 
   * @param diffuse_path path to diffuse map image
   * @param specular_path path to specular map image
   * @param normal_path path to normal map image
   * @return initilized `self`
   */
  slsMaterial *(*init)(slsMaterial *self,
                       char const *diffuse_path,
                       char const *specular_path,
                       char const *normal_path);

  void (*dtor)(slsMaterial *self);

  /**
   * @brief Program property.
   * @detail Methods for setting and getting active mesh shader program
   */
  void (*set_program)(slsMaterial *self, GLuint program);

  GLuint (*get_program)(slsMaterial *self);

  void (*bind)(slsMaterial *self);

  slsMaterial_p *priv;

  slsTexPair diffuse;
  slsTexPair specular;
  slsTexPair normal;

  kmVec3 diffuse_color;
  kmVec3 ambient_color;
  kmVec3 specular_color;

  float shininess;
};

slsMaterial const *sls_texture_class();

/**
 * @brief allocates and initializes a slsMaterial object.
 * @details all parameters (aside from `self`) are optional:
 * if a path parameter is NULL, the slsTexPair will be marked as
 * inactive
 * 
 * @param diffuse_path path to diffuse map image
 * @param specular_path path to specular map image
 * @param normal_path path to normal map image
 * @return initilized `self`
 */
slsMaterial *sls_texture_new(char const *diffuse_path,
                             char const *specular_path,
                             char const *normal_path);


GLuint sls_gltex_from_surface(SDL_Surface *surface);

/**
 * @brief generates a openGL texture handle from given path.
 *
 * @param path path of the image. Its file extension should correspond to the image format
 *
 * @param width_opt. If > 0, the texture width, else, the texture uses the image's native width
 * @param height_opt. If > 0, the texture height, else, the texture uses the image's native height
 */
GLuint sls_gltex_from_file(char const *path, int width_opt, int height_opt);


//---------------------------------image utils-------------------------------------

GLenum sls_glformat_from_sdlformat(SDL_PixelFormat const *fmt)
    SLS_NONNULL(1);


#endif //DANGERENGINE_SLSTEXTURE_H
