#version 130

varying vec4 frag_color;
varying vec2 frag_uv;


uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;


uniform float time;


void main()
{
  gl_FragColor = frag_color;
}
