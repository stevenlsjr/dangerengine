#line 2 0
/**
 * @file default.frag
 * @brief sample frag shader. Define SLS_MODERN_GL to use modern shaders.
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 **/


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

const float ambient_str = 0.1;


/**
 * a stupid hack. Right now, loaded PNG
 files have their color order as BGRA
 TODO(Steven): make sure textures load with correct pixel format
 */
vec4 bgra_to_rgba(vec4 color);
Light sample_source();


void main()
{
  Light l;
  l.position = vec4(0.0, 0.0, 4.0, 1.0);
  l.color = vec3(1.0, 1.0, 1.0);
  l.attenuation = 1.0;
  mat4 light_mv = lights.light_modelview[0];



  vec4 texel_color = bgra_to_rgba(texture(diffuse_tex, frag_uv));
  vec3 texel_norm = bgra_to_rgba(texture(specular_tex, frag_uv)).xyz;
  vec3 texel_spec = bgra_to_rgba(texture(specular_tex, frag_uv)).xyz;

  float shininess = (texel_spec.r + texel_spec.g + texel_spec.b) / 3.0;


  vec4 material_color = texel_color * frag_color;

  if (material_color.a < 0.001) {discard;}

  vec3 ambient = (l.color * material_color.rgb * ambient_str).xyz;

  // diffuse light


  if (length(texel_norm) <= 0.0) {
    texel_norm = frag_normal;
  }
  vec3 norm = normalize(normal_mat * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
  vec3 light_dir;

  if (l.position.w <= 1e-7) {
    light_dir =
          normalize((light_mv * l.position).xyz);
  } else {
    light_dir =
      normalize((light_mv * l.position).xyz - frag_pos);
  }

  float k_d = max(abs(dot(norm, light_dir)), 0.0);

  vec3 diffuse_color = l.color * k_d * material_color.rgb;

  // specular light

  vec3 specular_product = vec3(0.1, 0.1, 0.1);

  vec3 E = normalize(-frag_pos);
  vec3 R = normalize(-reflect(light_dir, norm));

  float k_s = pow(max(dot(R,E), 0.0), shininess);

  vec3 specular = k_s * specular_product;

  out_color = vec4(specular, material_color.a);
  //out_color = vec4(norm ,material_color.a);

}

vec4 bgra_to_rgba(vec4 color)
{
  return vec4(color.b, color.g, color.r, color.a);
}
