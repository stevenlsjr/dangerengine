
/*----------------------------*
 * gl4.1 shader
 *----------------------------*/

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;



out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_uv;
out vec3 frag_viewdir;



void main()
{

  frag_color = color;

  frag_normal  = normal;
  frag_viewdir = (normal_mat * vec4(normal, 0.0f)).xyz;

  frag_uv = uv;

  frag_pos = vec3(model_view * vec4(position, 1.0f));
  frag_pos.z = 0.0f;

  gl_Position = projection * model_view * vec4(position, 1.0f);
}



