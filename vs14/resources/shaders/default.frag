#version 410

out vec4 color;

in vec4 frag_color;
in vec2 frag_uv;

<<<<<<< HEAD

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;
=======
uniform mat4 model_view;
uniform mat4 normal_mat;
>>>>>>> 8ffdb773d1fdacb03adef3e0c5effb0f32a74c99

uniform float time;

uniform mat4 projection;

void main()
{
  
  color = frag_color;
<<<<<<< HEAD
}
=======
}
>>>>>>> 8ffdb773d1fdacb03adef3e0c5effb0f32a74c99
