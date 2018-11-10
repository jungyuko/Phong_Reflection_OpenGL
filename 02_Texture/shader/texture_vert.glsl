uniform mat4 u_pvm_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

varying vec3 v_vertex_wc;
varying vec3 v_normal_wc;
varying vec2 v_texcoord;

void main()
{
  v_vertex_wc = vec3(u_model_matrix * vec4(a_vertex,1));
  v_normal_wc = normalize(u_normal_matrix * a_normal);
  v_texcoord  = a_texcoord;
  gl_Position = u_pvm_matrix * vec4(a_vertex, 1);
}
