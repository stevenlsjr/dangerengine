#line 0 10
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


out vec4 out_color;

in vec3 frag_pos;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;

in float cloud_x_offset;

/**
 * a stupid hack. Right now, loaded PNG
 files have their color order as BGRA
 TODO(Steven): make sure textures load with correct pixel format
 */
vec4 bgra_to_rgba(vec4 color);

float cloud_value();


void main()
{
  int i=0;

  vec4 s1_texel = bgra_to_rgba(texture(diffuse_tex, frag_uv));
  vec4 s2_texel = bgra_to_rgba(texture(normal_tex, frag_uv));

  vec4 texel = mix(s1_texel, s2_texel, season_blend);

  vec4 a_product,
       d_product,
       s_product;

 float kd, ks, shininess;


  vec3 l;
  vec4 ambient, diffuse, specular;
  vec3 R,E;

  vec4 light_pos = lights.light_positions[i];
  light_pos = vec4(-100.0, 0.0, -100.0, 0.0);

  a_product = vec4(lights.ambient_products[i], 1.0)* texel;

  vec4 d_material = mix(texel, vec4(1.0, 1.0, 1.0, 1.0), cloud_value());
  d_product = d_material * vec4(lights.diffuse_products[i], 0.0);

  s_product = vec4(lights.specular_products[i], 1.0);
  //s_product = vec4(0.1, 0.1, 0.1, 1.0);

  if (light_pos.w == 0.0) {
    l = normalize(normalize(light_pos.xyz - frag_pos));
  } else {
    l = normalize((lights.light_modelview[i]*light_pos).xyz  - frag_pos);
  }

  ambient = a_product ;

  // diffuse

  kd = max((dot(frag_normal, l)), 0.0);
  diffuse = kd * d_product;

    // specular


  shininess = 2.0;
  E = normalize(-frag_pos);
  R = normalize(reflect(l, frag_normal));

  ks = kd > 0.0?
    pow(max(dot(R, E), 0.0), shininess):
    0.0;
  specular = ks * s_product;



  out_color = ambient + diffuse + specular;
  out_color.a = texel.a;

}
float cloud_value()
{


  //vec4 tex = texture(specular_tex, vec2(mod(frag_uv.x, 1.0), frag_uv.y));
  vec4 tex = texture(specular_tex, vec2(mod(cloud_x_offset, 1.0), frag_uv.y));

  float albedo = clamp((tex.r + tex.g + tex.b) / 3.0, 0.0, 1.0);

  return albedo;
}



vec4 bgra_to_rgba(vec4 color)
{
  return vec4(color.b, color.g, color.r, color.a);
}
