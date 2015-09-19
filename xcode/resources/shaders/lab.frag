
/*----------------------------*
 * gl4.1 shader
 *----------------------------*/

  out vec4 color;

  in vec4 frag_color;
  in vec2 frag_uv;


  uniform mat4 model_view;
  uniform mat4 normal_mat;
  uniform mat4 projection;

  uniform float time;
  uniform sampler2D diffuse_map;
  uniform sampler2D specular_map;
  uniform sampler2D normal_map;

  void main()
  {
    color = frag_color;
  }

