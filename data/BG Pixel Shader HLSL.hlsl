bool al_use_tex;
texture al_tex;
float angle;
float width;
sampler2D s = sampler_state {
   texture = <al_tex>;
};

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
   if (al_use_tex) {
      Input.TexCoord *= width;
      Input.TexCoord.x += (angle / 360.0f) * width;
      Input.TexCoord.x = (((Input.TexCoord.x % width) + width) % width);
      return Input.Color * tex2D(s, Input.TexCoord / width);
   }
   else {
      return Input.Color;
   }
}
