//
// Created by Steven on 6/22/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <string>


using namespace std;
using namespace testing;

class RenderTests: public Test {
protected:

  slsContext *ctx = nullptr;

  virtual void SetUp()
  {
    Test::SetUp();

    ctx = sls_context_new("title", 480, 480);
    sls_msg(ctx, setup);

  }

  virtual void TearDown()
  {
    sls_msg(ctx, dtor);



    Test::TearDown();
  }

};


TEST_F(RenderTests, ContextCreation)
{
  ASSERT_NE(nullptr, ctx);
}

TEST_F(RenderTests, ShaderRead)
{

  const auto source_path = string("/Users/Shea/Projects/dangerengine/");
  auto vs_path = source_path + string("resources/shaders/default.vert.glsl");
  auto fs_path = source_path + string("resources/shaders/default.frag.glsl");
  auto program = sls_create_program(vs_path.c_str(), fs_path.c_str());

  auto res = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &res);
  EXPECT_TRUE(res);

  glDeleteProgram(program);
}
