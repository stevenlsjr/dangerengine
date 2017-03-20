#line 0 10

/*----------------------------*
 * gl4.1 active_shader
 *----------------------------*/


out vec4 out_color;

in vec3 frag_pos;
in vec3 frag_eye;
in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal;



const vec4 ambp = vec4(vec3(1.0, 1.0, 1.0), 1.0);
const vec4 diffp = vec4(1.0, 0.5, 1.0, 1.0);
const vec4 specp = vec4(vec3(1.0, 1.0, 1.0) * 1, 1.0);


void main(void)
{



  out_color = diffp * frag_color;
  out_color.a = diffp.a; // set alpha to material transparency
}