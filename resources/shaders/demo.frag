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
in vec3 frag_eye;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;


/**
 * a stupid hack. Right now, loaded PNG
 files have their color order as BGRA
 TODO(Steven): make sure textures load with correct pixel format
 */

const vec4 ambp = vec4(vec3(1.0, 1.0, 1.0) * 0.05, 1.0);
const vec4 diffp = vec4(1.0, 0.5, 1.0, 1.0);
const vec4 specp = vec4(vec3(1.0, 1.0, 1.0) * 1, 1.0);

const vec3 c_light_dir = vec3(0.0, 1.0, -10.0);
const float shine = 20.0;

void main(void)
{
  vec3 light_dir = (model_view * vec4(c_light_dir, 1.0)).xyz;
  //vec3 light_dir = c_light_dir;
  vec3 L_term = normalize(light_dir);
  vec3 N_term = normalize(frag_normal);

  float lambert = dot(N_term, -L_term);
  vec4 ia = ambp,
  id = vec4(0.0, 0.0, 0.0, 1.0),
  is = vec4(0.0, 0.0, 0.0, 1.0);



  if (lambert > 0.0) {
    id = diffp * lambert;
    vec3 eye = normalize(frag_eye);
    vec3 reflect = reflect(L_term, N_term);
    float spec_term = pow(max(dot(reflect, eye), 0.0), shine);
    is = spec_term * specp;
  }

  out_color = ia + id + is;
  out_color.a = diffp.a; // set alpha to material transparency
}