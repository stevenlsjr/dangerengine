/**
 * @file uniforms.glsl
 * @brief dives uniform values for all shaders
 * @license FreeBSD
 **/

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform int z_layer;

