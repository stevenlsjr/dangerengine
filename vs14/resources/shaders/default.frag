#version 410

out vec4 color;

in vec4 frag_color;
in vec2 frag_uv;

uniform float time;

void main()
{
  float timer = sin(time * 100);
  float timeB = time/10.0;

  mat4 rot = mat4(
    vec4(cos(timeB), -sin(timeB), 0.0, 0.0),
    vec4(sin(timeB),  cos(timeB), 0.0, 0.0),
    vec4(0.0,       0.0,        1.0, 0.0),
    vec4(0.0,       0.0,        0.0, 1.0)
  );

  vec4 loc = vec4(frag_uv, 1.0, 1.0) * rot;
  vec4 ripple = sin(loc * 100) * cos(loc * sin(time) * 100);
  color = frag_color/2 + ripple/2;
}