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

struct slsTexture {
  slsTexture *(*init)(slsTexture *self);

  void (*dtor)(slsTexture *self);

  void (*bind_texture)(GLuint program, GLuint uniform_location);

  GLuint tex_object;
};

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
 * @returns last error or IL_NO_ERROR if no errors reported
 */
ILenum sls_il_get_errors();

#endif //DANGERENGINE_SLSTEXTURE_H
