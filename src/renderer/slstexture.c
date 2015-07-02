//
// Created by Steven on 7/1/15.
//

#include "slstexture.h"

GLuint sls_gltex_from_file(char const *path, int width_opt, int height_opt) {

  ILuint img;
  GLuint tex;
  int width, height, depth, bpp;
  ILenum il_format, il_type;
  GLenum gl_format, gl_type;


  il_format = IL_RGBA;
  gl_format = GL_RGBA;

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

ILenum sls_il_get_errors() {
  ILenum err, last;
  last = IL_NO_ERROR;
  while ((err = ilGetError()) != IL_NO_ERROR) {
    sls_log_warn("devIL: %s", ilGetString(err));
    if (err == IL_OUT_OF_MEMORY) {
      sls_log_fatal("out of memory error!");
    }

    if (last == IL_NO_ERROR) {
      last = err;
    }
  }

  return last;
}
