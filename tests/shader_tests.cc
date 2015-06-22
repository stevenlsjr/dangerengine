//
// Created by Steven on 6/17/15.
//

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#include "../src/dangerengine.h"


using namespace testing;
using namespace std;

class ShaderTests : public Test {
protected:
  GLuint vs, fs, program;
  string vs_src =
    "#version 400"
      "layout (location = 0) in vec3 vert_pos;"

      "void main() {"
      "gl_Position = vec4(vert_pos, 1.0)"
      "}";

  slsContext *ctx = nullptr;

  virtual void SetUp() override
  {
    ctx = sls_context_new("title", 200, 200);

    sls_bind_context(ctx);
    ASSERT_NE(nullptr, ctx);

    Test::SetUp();
  }

  virtual void TearDown() override
  {
    sls_msg(ctx, dtor);
    Test::TearDown();
  }
};


TEST_F(ShaderTests, ReadFile)
{
  auto src = string("hello world\n");
  auto path = "readFile.tmp";

  FILE *f = nullptr;
  f = fopen(path, "w");
  ASSERT_NE(nullptr, f);

  fputs(src.c_str(), f);

  fclose(f);

  auto read_src = sls_read_file(path);
  EXPECT_STREQ(src.c_str(), read_src);

}

TEST_F(ShaderTests, CompileVs)
{
  slsBool res;
  vs = sls_compile_shader(vs_src.c_str(), GL_VERTEX_SHADER, &res);
  EXPECT_EQ(SLS_TRUE, res);
}
