layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_uv;


void main() {
	gl_PointSize = 4.0;

  frag_color = color;
  frag_normal = normalize(normal);
  frag_uv = uv;

  frag_pos = position;

  gl_Position = vec4(frag_pos, 1.0f);
}
