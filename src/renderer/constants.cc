/**
 * @file ${FILE}
 * @brief C++ source for raw-character shader sources
 * @license ${LICENSE}
 * Copyright (c) 12/23/17, Steven
 * 
 **/


char const *SLS_DEFAULT_VS = R"SHADER(

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

out vec3 frag_pos;
out vec3 frag_normal;
out vec3 frag_eye;
out vec4 frag_color;
out vec2 frag_uv;


void main(void)
{
  gl_PointSize = 4.0;

  frag_color = color;

  vec3 nt = (normal_mat * vec4(normal, 0.0)).xyz;
  frag_normal = normalize(nt);

  frag_uv = uv;
  frag_pos = vec3(model_view * vec4(position, 1.0));
  frag_eye = -frag_pos;


  gl_Position = projection * vec4(frag_pos, 1.0);
}
)SHADER";

char const *SLS_DEFAULT_FS = R"SHADER(
#line 0 10

/*----------------------------*
 * gl4.1 active_shader
 *----------------------------*/


out vec4 out_color;

in vec3 frag_pos;
in vec3 frag_eye;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;



const vec4 ambp = vec4(vec3(1.0, 1.0, 1.0), 1.0);
const vec4 diffp = vec4(1.0, 0.5, 1.0, 1.0);
const vec4 specp = vec4(vec3(1.0, 1.0, 1.0) * 1, 1.0);


void main(void)
{



  out_color = diffp * frag_color;
  out_color.a = diffp.a; // set alpha to material transparency
}
)SHADER";
char const *SLS_DEFAULT_UNIFORMS = R"SHADER(
#line 0 0
/**
 * @file uniforms.glsl
 * @brief dives uniform values for all shaders
 * @license FreeBSD
 **/

uniform mat4 projection;
uniform mat4 model_view;
uniform mat4 inv_model_view;
uniform mat4 normal_mat;

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;

uniform Material {
  vec3 specular_color;
  vec3 diffuse_color;
  vec3 ambient_color;

  float shininess;
} material;


)SHADER";