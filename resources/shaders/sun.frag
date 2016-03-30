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
 *    and/or other materials provided with the dist.
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

const float ambient_str = 0.1;


/**
 * a stupid hack. Right now, loaded PNG
 files have their color order as BGRA
 TODO(Steven): make sure textures load with correct pixel format
 */
vec4 bgra_to_rgba(vec4 color);


void main()
{


  int i=0;

  vec4 texel = bgra_to_rgba(texture(diffuse_tex, frag_uv));

  vec4 a_product,
       d_product,
       s_product;


  vec3 l;
  vec4 ambient, diffuse;
  vec3 R,E;

  vec4 light_pos;
  light_pos = lights.light_positions[i];

  a_product = vec4(lights.ambient_products[i], 1.0);
  d_product = texel * vec4(lights.diffuse_products[i], 0.0);
  s_product = vec4(lights.specular_products[i], 1.0);

  if (light_pos.w == 0.0) {
    l = normalize(normalize(light_pos.xyz) - frag_pos);
  } else {
    l = normalize((lights.light_modelview[i]*light_pos).xyz - frag_pos);
  }

  //E = normalize(-frag_pos);
  //R = normalize(-reflect(l, frag_normal));

  // compute light terms

  ambient = a_product;

  float kd = max(abs(dot(frag_normal, l)), 0.0);
  diffuse = kd * texel;

  out_color = vec4(frag_pos, 1.0);
  out_color.a = texel.a;

}



vec4 bgra_to_rgba(vec4 color)
{
  return vec4(color.b, color.g, color.r, color.a);
}
