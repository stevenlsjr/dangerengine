//
// Created by Steven on 7/1/15.
//

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
   * @brief flags that the texture is bound to a shader uniform
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

  void (*set_program)(slsTexture *self, GLuint program);

  void (*bind)(slsTexture *self);

  GLuint program;

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
