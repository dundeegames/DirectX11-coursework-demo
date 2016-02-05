// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)
#define MAX_LIGHTS 4



Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


cbuffer LightBuffer : register(cb0)
{
  float4 diffuseColour[MAX_LIGHTS];
  //float4 ambientColour;
  float4 lightPosition[MAX_LIGHTS];
};

struct InputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
  float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
  float4 textureColour = (float4)0; // object colour based only on texture
  float3 lightDir = (float3)0;
  float lightIntensity = (float)0;
  float4 finalColour = (float4)0;


  // Set the default output colour to the ambient light value for all pixels.
  //finalColour = ambientColour;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColour = shaderTexture.Sample(SampleType, input.tex);

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    // Invert the light direction for calculations.
    lightDir = normalize(input.position3D - lightPosition[i].xyz);

    // Calculate the amount of light on this pixel.
    // saturate() clamps value between 0-1
    lightIntensity = saturate(dot(input.normal, -lightDir));

    if (lightIntensity > 0.0f)
    {
      // Determine the final amount of diffuse color based on the diffuse color combined
      //with the light intensity.
      //commentiong out this line will remove the diffuse light
      finalColour += saturate(diffuseColour[i] * lightIntensity);

    }
  }



  // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
  //finalColour *= textureColour;


  return finalColour;
}

