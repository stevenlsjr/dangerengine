/**
 * @file default.frag
 * @brief sample frag shader. Define SLS_MODERN_GL to use modern shaders.
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of Steven Shea. 
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
  l.position = vec4(0.0, 0.0, 3.0, 1.0);
  l.color = vec3(1.0, 1.0, 1.0);
  l.attenuation = 1.0;

  vec4 texel_color = bgra_to_rgba(texture(diffuse_map, frag_uv));
  vec3 texel_norm = bgra_to_rgba(texture(normal_map, frag_uv)).xyz;
  vec3 texel_spec = bgra_to_rgba(texture(specular_map, frag_uv)).xyz;

  float shininess = (texel_spec.r + texel_spec.g + texel_spec.b) / 3.0;


  vec4 material_color = texel_color * frag_color;

  if (material_color.a < 0.001) {discard;}

  vec3 ambient = (l.color * material_color.rgb * ambient_str).xyz;

  // diffuse light


  vec3 norm = normalize(normal_mat * vec4(texel_norm, 0.0)).xyz;
  //vec3 norm = frag_normal;
  vec3 light_dir;
  if (l.position.w == 0.0) {
    light_dir =
      normalize(l.position.xyz - frag_pos);
  } else {
    light_dir =
      normalize((model_view * l.position).xyz - frag_pos);
  }

  float k_d = max(abs(dot(norm, light_dir)), 0.0);

  vec3 diffuse_color = k_d * l.color *  material_color.xyz;

  // specular light

  vec3 specular_product = vec3(0.1, 0.1, 0.1);

  vec3 E = normalize(-frag_pos);
  vec3 R = normalize(-reflect(light_dir, norm));

  float k_s = pow(max(dot(R,E), 0.0), shininess);

  vec3 specular = (dot(light_dir, frag_normal) < 0.0)?
    vec3(0.0, 0.0, 0.0):
    k_s * specular_product;

  out_color = vec4(diffuse_color, material_color.a);
  //out_color = vec4(norm ,material_color.a);

}



vec4 bgra_to_rgba(vec4 color)
{
  return vec4(color.b, color.g, color.r, color.a);
}
