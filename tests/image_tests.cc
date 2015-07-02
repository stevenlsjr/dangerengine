//
// Created by Steven on 7/2/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <string>
#include <tuple>
#include <IL/il.h>

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
    ctx = sls_context_new("image tests", 100, 100);
    sls_msg(ctx, setup);

    size = make_tuple(1080, 1080);

    path = "resources/1080.jpeg";

    glGenTextures(1, tex);
  }

  virtual ~ImageTests() {
    if (ctx) {
      sls_msg(ctx, dtor);
    }

    glDeleteTextures(1, &tex);
  }
};


TEST_F(ImageTests, ImageCreation) {
  ILuint img;
  ilGenImages(1, &img);
  ilBindImage(img);

  EXPECT_TRUE(ilLoadImage(path.c_str()))
      << "image failed to load";

  tuple<int, int> i_size = make_tuple(ilGetInteger(IL_IMAGE_WIDTH),
                                      ilGetInteger(IL_IMAGE_HEIGHT));

  EXPECT_EQ(size, i_size)
       << "retrieved image size is not equal to actual size";

  ilDeleteImage(img);
}

