
/*----------------------------*
 * gl4.1 shader
 *----------------------------*/

struct Light {
  vec4 position;
  vec3 color;
  float attenuation;
};

out vec4 out_color;

in vec3 frag_pos;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;

in vec3 frag_transformed_normal;

uniform mat4 model_view;
uniform mat4 normal_mat;
uniform mat4 projection;

uniform float time;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;

const float ambient_str = 0.01;


/**
 * a stupid hack. Right now, loaded PNG
 files have their color order as BGRA
 TODO(Steven): make sure textures load with correct pixel format
 */
vec4 bgra_to_rgba(vec4 color);
Light sample_source();


void main()
{
  Light l = sample_source();
  vec4 texel_color = bgra_to_rgba(texture(diffuse_map, frag_uv));
  vec3 texel_norm = bgra_to_rgba(texture(normal_map, frag_uv)).xyz;
  vec3 texel_spec = bgra_to_rgba(texture(specular_map, frag_uv)).xyz;

  vec4 material_color = texel_color * frag_color;

  if (material_color.a < 0.001) {discard;}

  out_color = material_color;

  //color = norm;
}


Light sample_source()
{
  Light l;

  l.position = vec4(0.0, 0.5, 0.2, 1.0);
  l.color = vec3(1.0, 1.0, 1.0);
  l.attenuation = 1.0;

  return l;
}


vec4 bgra_to_rgba(vec4 color)
{
  return vec4(color.b, color.g, color.r, color.a);
}
