// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);


cbuffer stateBuffer : register(cb0)
{
  float3 colour;
  int renderState;
};


struct InputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
  float4 textureColor;
  float4 finalColor;
  float colourChannel;

  float4 baseColor = float4(colour, 1.0f);;


  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = texture0.Sample(Sampler0, input.tex);

  switch (renderState)
  {
  case 0: // NORMAL
    finalColor = textureColor;
    break;

  case 1: // GRAYSCALE
    colourChannel = (0.299*textureColor.r + 0.587*textureColor.g + 0.114f*textureColor.b);
    finalColor = float4(colourChannel, colourChannel, colourChannel, 1.0f);
    break;

  case 2: // INVERSE
    finalColor = float4(1.0f - textureColor.r, 1.0f - textureColor.g, 1.0f - textureColor.b, 1.0f);
    break;

  case 3: // COLOURED
    finalColor = lerp(textureColor, baseColor, 0.5);
    break;

  default:
    finalColor = float4(1.0f, 0.0f, 0.0f, 1.0f);  //error red
    break;

  }
  
  return finalColor;
}