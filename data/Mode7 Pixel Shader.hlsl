// A Modified Allegro 5 default Pixel Shader.
// All modifications made by Edward Opich.

bool al_use_tex;
texture al_tex;
texture map_tex;
texture oob_tex;
float3 gamePos;
float horizon;
float4x4 transform;

sampler2D s = sampler_state {
   texture = <map_tex>;
};

sampler2D o = sampler_state {
   texture = <oob_tex>;
};

float4 ps_main(VS_OUTPUT Input, float2 screenPos : VPOS) : COLOR0
{
   if (al_use_tex) {
      // Everything here is in pixels!
      Input.TexCoord.x = (Input.TexCoord.x * 256.0f);
      Input.TexCoord.y = (Input.TexCoord.y * (224.0f - 32.0f));

      // Center the view
      Input.TexCoord.x -= 128.0f;
      Input.TexCoord.y -= (112.0f - 16.0f);

      // Apply Transform
      Input.TexCoord = mul(Input.TexCoord, transform);

      // Mode 7!
      Input.TexCoord.x = ((Input.TexCoord.x / ((screenPos.y - horizon) / -gamePos.z)));
      Input.TexCoord.y = ((Input.TexCoord.y / ((screenPos.y - horizon) / -gamePos.z)));

      // Move view to x and y
      Input.TexCoord.x += gamePos.x;
      Input.TexCoord.y += gamePos.y;

      // If we're on the track, draw some track pixels...
      if(Input.TexCoord.x >= 0.0f && Input.TexCoord.x < 1024.0f && Input.TexCoord.y >= 0.0f && Input.TexCoord.y < 1024.0f)
      {
         return Input.Color * tex2D(s, Input.TexCoord / 1024.0f);
      }
      // ... Otherwise, draw some of the out-of-bounds pixels.
      else
      {
         Input.TexCoord = ((Input.TexCoord % 8) + 8) % 8;
         Input.TexCoord *= 64.0f;
         return Input.Color * tex2D(o, Input.TexCoord / 1024.0f);
      }
   }
   else {
      return Input.Color;
   }
}

technique lol
{
  pass P0
  {
    PixelShader = compile ps_3_0 ps_main();
    VertexShader = compile vs_3_0 vs_main();
  }
}
