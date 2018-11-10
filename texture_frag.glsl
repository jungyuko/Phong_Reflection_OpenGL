uniform sampler2D u_texid;
uniform sampler2D u_texid_frame;

varying vec2 v_texcoord;

void main() 
{
  vec4 color1 = texture2D(u_texid, v_texcoord);
  vec4 color2 = texture2D(u_texid_frame, v_texcoord);

  gl_FragColor = color1*0.5 + color2*0.5;
}

