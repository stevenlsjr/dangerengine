/**
 * @file uniforms.glsl
 * @brief dives uniform values for all shaders
 * @license FreeBSD
 **/

#define SLS_N_LIGHTS 8

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;


uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform int z_layer;

uniform Material {
  vec3 specular_color;
  vec3 diffuse_color;
  vec3 ambient_color;

  float shininess;
} material;

struct Lights {
  int n_lights;
  vec3 ambient_products[SLS_N_LIGHTS];
  vec3 diffuse_products[SLS_N_LIGHTS];
  vec3 specular_products[SLS_N_LIGHTS];
  vec4 light_positions[SLS_N_LIGHTS];
  mat4 light_modelview[SLS_N_LIGHTS];

};

uniform Lights lights;
