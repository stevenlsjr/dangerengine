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

#ifndef DANGERENGINE_SLSTEXTURE_H
#define DANGERENGINE_SLSTEXTURE_H

#include <IL/il.h>
#include "../sls-gl.h"
#include "../slsutils.h"
#include <inttypes.h>

typedef struct slsTexture slsTexture;
typedef struct slsTexture_p slsTexture_p;
typedef struct slsTexPair slsTexPair;

struct slsTexPair {
  GLuint unit;
  GLuint uniform;
  /**
   * @brief flags the texture's successful load in memory
   */
  slsBool is_active;

  /**
   * @brief flags that the texture is bound to a shader uniform.
   * @detail Will be SLS_FALSE if associated variable is not in shader program, or texture has not yet been bound
   */
  slsBool is_bound;
};

struct slsTexture {
  /**
   * @brief initializes a slsTexture object.
   * @details all parameters (aside from `self`) are optional:
   * if a path parameter is NULL, the slsTexPair will be marked as
   * inactivek
   * 
   * @param diffuse_path path to diffuse map image
   * @param specular_path path to specular map image
   * @param normal_path path to normal map image
   * @return initilized `self`
   */
  slsTexture *(*init)(slsTexture *self, 
                      char const *diffuse_path,
                      char const *specular_path,
                      char const *normal_path);

  void (*dtor)(slsTexture *self);

  /**
   * @brief Program property.
   * @detail Methods for setting and getting active mesh shader program
   */
  void (*set_program)(slsTexture *self, GLuint program);

  GLuint (*get_program)(slsTexture *self);

  void (*bind)(slsTexture *self);

  slsTexture_p *priv;


  slsTexPair diffuse;
  slsTexPair specular;
  slsTexPair normal;
};

slsTexture const *sls_texture_class();

/**
 * @brief allocates and initializes a slsTexture object.
 * @details all parameters (aside from `self`) are optional:
 * if a path parameter is NULL, the slsTexPair will be marked as
 * inactive
 * 
 * @param diffuse_path path to diffuse map image
 * @param specular_path path to specular map image
 * @param normal_path path to normal map image
 * @return initilized `self`
 */
slsTexture *sls_texture_new(char const *diffuse_path,
                            char const *specular_path,
                            char const *normal_path);

/**
 * @brief generates a openGL texture handle from given path.
 *
 * @param path path of the image. Its file extension should correspond to the image format
 *
 * @param width_opt. If > 0, the texture width, else, the texture uses the image's native width
 * @param height_opt. If > 0, the texture height, else, the texture uses the image's native height
 */
GLuint sls_gltex_from_file(char const *path, int width_opt, int height_opt);

/**
 * @brief reads through the devIL error stack and reports errors.
 * @detail will abort program if out of memory occurs
 *
 * @returns top error or IL_NO_ERROR if no errors reported
 */
ILenum sls_il_get_errors();

#endif //DANGERENGINE_SLSTEXTURE_H
