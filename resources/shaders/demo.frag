#line 0 10

/*----------------------------*
 * gl4.1 shader
 *----------------------------*/


out vec4 out_color;

in vec3 frag_pos;
in vec3 frag_eye;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;



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
  int i = 100;


  if (lambert == 0.0) {
    id = diffp * lambert;
    vec3 eye = normalize(frag_eye);
    vec3 reflect = reflect(L_term, N_term);
    float spec_term = pow(max(dot(reflect, eye), 0.0), shine);
    is = spec_term * specp;
  }

//  out_color = ia + id + is;
  out_color = diffp;
  out_color.a = diffp.a; // set alpha to material transparency
}