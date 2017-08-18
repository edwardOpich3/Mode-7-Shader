#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D al_tex;
uniform sampler2D map_tex;
uniform sampler2D oob_tex;

uniform vec3 gamePos;
uniform float horizon;
uniform mat4 transform;

uniform bool al_use_tex;
varying vec4 varying_color;
varying vec2 varying_texcoord;

void main()
{
  if (al_use_tex)
  {
    vec2 myCoord = varying_texcoord;
    myCoord.x = (myCoord.x * 256.0);
    myCoord.y = (myCoord.y * (224.0 - 32.0));

    myCoord.x -= 128.0;
    myCoord.y -= (112.0 - 16.0);

    myCoord = myCoord * (mat2)transform;

    myCoord.x = ((myCoord.x / (((224.0 - gl_FragCoord.y) - horizon) / -gamePos.z)));
    myCoord.y = ((myCoord.y / (((224.0 - gl_FragCoord.y) - horizon) / -gamePos.z)));

    myCoord.x += gamePos.x;
    myCoord.y += -(gamePos.y - 1024.0);

    if(myCoord.x >= 0.0 && myCoord.x < 1024.0 && myCoord.y >= 0.0 && myCoord.y < 1024.0)
    {
        gl_FragColor = varying_color * texture2D(map_tex, myCoord / 1024.0);
    }
    else
    {
        myCoord = mod((mod(myCoord, 8) + 8), 8);
        myCoord *= 64.0;
        gl_FragColor = varying_color * texture2D(oob_tex, myCoord / 1024.0);
    }
  }
  else
    gl_FragColor = varying_color;
}
