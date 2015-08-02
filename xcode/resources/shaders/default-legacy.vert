
#version 130

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec4 color;

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;

varying vec4 frag_color;
varying vec2 frag_uv;

void main()
{
  frag_color = color;
  frag_uv = uv;
  gl_Position = projection * model_view * vec4(position, 1.0f);
}
