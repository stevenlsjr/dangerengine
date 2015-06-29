#version 410

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;

<<<<<<< HEAD
=======
uniform mat4 projection;

>>>>>>> 8ffdb773d1fdacb03adef3e0c5effb0f32a74c99
out vec4 frag_color;
out vec2 frag_uv;

void main()
{
  frag_color = color;
  frag_uv = uv;
<<<<<<< HEAD
  gl_Position =  projection * rot * vec4(position, 1.0f) ;
=======
  gl_Position = projection * vec4(position, 1.0f);
>>>>>>> 8ffdb773d1fdacb03adef3e0c5effb0f32a74c99
}
