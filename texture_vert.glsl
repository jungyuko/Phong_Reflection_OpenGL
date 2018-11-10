uniform mat4 u_pvm_matrix;

attribute vec3 a_vertex;
attribute vec2 a_texcoord;

varying vec3 v_vertex;
varying vec2 v_texcoord;

void main() 
{ 
  v_texcoord = a_texcoord;
  gl_Position = u_pvm_matrix * vec4(a_vertex, 1);
}
