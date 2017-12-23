/**
 * @file ${FILE}
 * @brief C++ source for raw-character shader sources
 * @license ${LICENSE}
 * Copyright (c) 12/23/17, Steven
 * 
 **/


char const *SLS_DEFAULT_VS = R"SHADER(

layout (location=0) in vec3 position;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

out vec4 frag_color;
out vec2 frag_uv;

void main(void)
{
  frag_color = color;

  frag_uv = uv;
  gl_Position = vec4(position, 1.0);
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


void main(void)
{

  out_color = vec4(1.0, 0.0, 1.0, 1.0);
}

)SHADER";
char const *SLS_DEFAULT_UNIFORMS = R"SHADER(
#line 0 0
/**
 * @file uniforms.glsl
 * @brief dives uniform values for all shaders
 * @license FreeBSD
 **/

uniform mat3 modelview_projection;

uniform sampler2D diffuse_tex;


)SHADER";