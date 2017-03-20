
layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

out vec3 frag_pos;
out vec3 frag_normal;
out vec3 frag_eye;
out vec4 frag_color;
out vec2 frag_uv;


void main() {
  gl_Position = vec4(position, 1.0);
  frag_pos = position;
}
