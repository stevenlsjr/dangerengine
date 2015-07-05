//
// Created by Steven on 7/1/15.
//

#include "slstexture.h"

/*----------------------------*
 * prototypes
 *----------------------------*/

slsTexture *sls_texture_init(slsTexture *self, 
                             char const *diffuse_path,
                             char const *specular_path,
                             char const *normal_path);

void sls_texture_dtor(slsTexture *self);

void sls_texture_set_program(slsTexture *self, GLuint program);
void sls_texture_bind(slsTexture *self);

/*----------------------------*
 * class implementation
 *----------------------------*/

slsTexture const *sls_texture_class() {

  static slsTexture proto = {
    .init=sls_texture_init,
    .dtor=sls_texture_dtor,
    .set_program = sls_texture_set_program,
    .bind=sls_texture_bind
  };
  return &proto;
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
  if (!self) {return NULL;}

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

  glUseProgram;
  for (int i=0; 
       i<sizeof(tups)/sizeof(struct tmp_tuple); 
       ++i) {

    struct tmp_tuple const *tup = tups + i;

    if (tup->path) {
      tup->pair->unit = sls_gltex_from_file(tup->path, 
                                            width, 
                                            height);

      tup->pair->is_active = SLS_TRUE;

    } else {
      tup->pair->is_active = SLS_FALSE;
    }

  }


  return self;

error:
  if (self && self->dtor) {
    sls_msg(self, dtor);
  }
}

void sls_texture_dtor(slsTexture *self)
{
  if (!self) {return ;}


  free(self);
}

void sls_texture_set_program(slsTexture *self, GLuint program)
{
  if (!self) {return;}
  sls_check(glIsProgram, "object %ui is not a program handle", program);

  self->diffuse.uniform = glGetUniformLocation(program, "diffuse");
  self->specular.uniform = glGetUniformLocation(program, "specular");
  self->normal.uniform = glGetUniformLocation(program, "normal");

  return;
error:
  return;
}


void sls_texture_bind(slsTexture *self)
{
  if (!self) {return;}
  glUseProgram(self->program);

  if (self->diffuse.is_active) {}
  if (self->specular.is_active) {}
  if (self->normal.is_active) {}
}


/*----------------------------*
 * utility functions
 *----------------------------*/

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

  gl_format = (il_format == IL_RGBA)? GL_RGBA: GL_RGB;


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
