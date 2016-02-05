// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour;
    float4 ambientColour;
    float3 lightDirection;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
  //float4 textureColour;
  float4 albedo = float4(0.5f, 0.5f, 0.5f, 1.0f); // object colour (dif+tex) without light
  float3 lightDir;
  float lightIntensity;
  //float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
  float4 colour = ambientColour;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  //textureColour = shaderTexture.Sample(SampleType, input.tex);
	
  // Invert the light direction for calculations.
  lightDir = -lightDirection;

  // Calculate the amount of light on this pixel.
  lightIntensity = saturate(dot(input.normal, lightDir));

  // Determine the final amount of diffuse color based on the diffuse color combined
  //with the light intensity.
  //commentiong out this line will remove the diffuse light
  colour += saturate(diffuseColour * lightIntensity);

  // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
  //colour = colour * textureColour;
  colour = colour * albedo;

	return colour;
}

