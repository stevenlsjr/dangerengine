
/**
 * @file default.vert
 * @brief sample vert shader. Define SLS_MODERN_GL to use modern shaders.
 * @ License FreeBSD
 * NOTE: don't declare uniforms in shader source.
 * string defining uniforms declared in renderer/shaderutils.c
 **/

/*----------------------------*
 * gl4.1 shader
 *----------------------------*/

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 color;

out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_uv;

out float cloud_x_offset;

void main()
{

  gl_PointSize = 4.0;

  frag_color = color;

  vec3 nt = (normal_mat * vec4(normal, 0.0)).xyz;
  frag_normal = normalize(nt);


  frag_uv = uv;

  cloud_x_offset = frag_uv.s + time /2.0;

  frag_pos = vec3(model_view * vec4(position, 1.0));

  gl_Position = projection * model_view * vec4(position, 1.0f);


}




