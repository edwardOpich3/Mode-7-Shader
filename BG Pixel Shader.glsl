#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D al_tex;
uniform bool al_use_tex;

uniform float angle;
uniform float width;

varying vec4 varying_color;
varying vec2 varying_texcoord;

void main()
{
  if (al_use_tex)
  {
	vec2 myCoord = varying_texcoord;
    myCoord *= width;
    myCoord.x += (angle / 360.0) * width;
    myCoord.x = mod((mod(myCoord.x,  width) + width), width);
    gl_FragColor = varying_color * texture2D(al_tex, myCoord / width);
  }
  else
    gl_FragColor = varying_color;
}
