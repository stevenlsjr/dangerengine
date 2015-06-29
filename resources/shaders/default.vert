#version 410

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;

out vec4 frag_color;
out vec2 frag_uv;

void main()
{

  float timeB = -time;

    mat4 rot = mat4(
      vec4(cos(timeB), -sin(timeB), 0.0, 0.0),
      vec4(sin(timeB),  cos(timeB), 0.0, 0.0),
      vec4(0.0,       0.0,        1.0, 0.0),
      vec4(0.0,       0.0,        0.0, 1.0)
    );
  frag_color = color;
  frag_uv = uv;
  gl_Position =  projection * rot * vec4(position, 1.0f) ;
}
