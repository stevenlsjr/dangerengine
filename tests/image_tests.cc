//
// Created by Steven on 7/2/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <string>
#include <tuple>

using namespace std;
using namespace testing;


class ImageTests : public ::testing::Test {
protected:
  slsContext *ctx;
  string path;
  tuple<int, int> size;
  GLuint tex;


public:
  ImageTests() {
    ctx = sls_context_new((char const *) "image tests", 100Lu, 100Lu);
    sls_msg(ctx, setup);

    size = make_tuple(1080, 1080);

    path = "resources/1080.jpeg";

    glGenTextures(1, &tex);

    //
  }

  virtual ~ImageTests() {
    if (ctx) {
      sls_msg(ctx, dtor);
    }

    glDeleteTextures(1, &tex);
  }
};

/**
 * @brief asserts that generated texture
 * is a valid openGL texture handle
 */
TEST_F(ImageTests, ValidTextureInstance) {
  glBindTexture(GL_TEXTURE_2D, tex);
  EXPECT_TRUE(glIsTexture(tex));
  glBindTexture(GL_TEXTURE_2D, 0);
}


TEST_F(ImageTests, TexCreation) {
  // push away gl errors
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    //sls_log_warn("gl error: 0x%x %i", err);
  }
  tex = sls_gltex_from_file(path.c_str(), get<0>(size), get<1>(size));
  glBindTexture(GL_TEXTURE_2D, tex);

  glBindTexture(GL_TEXTURE_2D, 0);


}
