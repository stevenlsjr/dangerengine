#version 410

out vec4 color;

in vec4 frag_color;
in vec2 frag_uv;


uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;
uniform sampler2D tex_sample;

void main()
{
  color = texture(tex_sample, frag_uv) * frag_color;
}


