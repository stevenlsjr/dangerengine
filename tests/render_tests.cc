//
// Created by Steven on 6/22/15.
//

#include <gtest/gtest.h>
#include "../src/dangerengine.h"
#include <string>
#include "test-utils.h"

using namespace std;
using namespace testing;


class RenderTests : public Test {
private:

  static slsMaterial *make_texture()
  {
    char const *img_path = "resources/art/barrelBeige_outline.png";
    char const *norm_path = "resources/art/barrelBeige_normal.png";

    return sls_texture_new(img_path,
                           nullptr,
                           norm_path);
  }

  static GLuint make_program()
  {
    char const *vs_path_legacy = "resources/shaders/default-legacy.vert";
    char const *fs_path_legacy = "resources/shaders/default-legacy.frag";
    char const *vs_path = "resources/shaders/default.vert";
    char const *fs_path = "resources/shaders/default.frag";

    char const *uniform_path = "resources/shaders/uniforms.glsl";

    GLuint p = 0;

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    p = sls_create_program(vs_path, fs_path, uniform_path);

    return p;
  }


protected:

  slsContext *ctx = nullptr;
  slsMesh *mesh = nullptr;
  slsMaterial *tex = nullptr;
  GLuint program;

  slsShader shader;


  virtual void SetUp()
  {
    sls_init();
    ctx = sls_context_new("title", 480, 480);
    ASSERT_TRUE(ctx) << "ctx is null";
    sls_msg(ctx, setup);

    mesh = sls_mesh_square();
    tex = make_texture();

    program = make_program();
    sls_shader_init(&shader, ctx->pool, program);


    sls_msg(tex, set_program, program);
    sls_msg(mesh, bind, &shader);


  }

  virtual void TearDown()
  {

    if (mesh) {
      sls_msg(mesh, dtor);
    }
    if (tex) {
      sls_msg(tex, dtor);
    }


    shader.dtor(&shader);
    glDeleteProgram(program);


    sls_msg(ctx, dtor);
    sls_terminate();
  }

};


TEST_F(RenderTests, ContextCreation)
{
  ASSERT_TRUE(ctx) << "context is null";
}

TEST_F(RenderTests, ShaderRead)
{
  auto res = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &res);
  EXPECT_TRUE(res);
}


TEST_F(RenderTests, Mesh)
{
  EXPECT_TRUE(mesh);
  EXPECT_TRUE(glIsVertexArray(mesh->vao));
  EXPECT_TRUE(glIsBuffer(mesh->ibo));
  EXPECT_TRUE(glIsBuffer(mesh->vbo));
}

TEST_F(RenderTests, TextureSetGetProgram)
{
  ASSERT_TRUE(tex);
  EXPECT_EQ(program, sls_msg(tex, get_program));
}


TEST_F(RenderTests, TextureComponents)
{
  // tex should have a diffuse and normal, not a specular
  EXPECT_TRUE(tex->diffuse.is_active);
  EXPECT_TRUE(tex->normal.is_active);
  EXPECT_FALSE(tex->specular.is_active);

  sls_msg(tex, bind);

}

TEST_F(RenderTests, TestGeometries)
{
  auto n_verts = 10;
  kmVec4 color = {1.0, 1.0, 1.0, 1.0};
  slsVertex *sphere = nullptr;
  slsMesh *m = nullptr;
  sphere = sls_sphere_vertices(n_verts, &color);
  m = sls_sphere_mesh(n_verts, &color);

  ASSERT_TRUE(sphere);
  ASSERT_TRUE(m);

  free(sphere);
  m->dtor(m);
}


