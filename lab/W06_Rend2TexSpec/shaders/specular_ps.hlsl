// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour;
    float4 ambientColour;
    float3 lightDirection;
    float specularPower;
    float4 specularColour;
    bool hasTexture;
    bool padding[15];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
  // albedo = object colour (dif+tex) without light
  //float4 albedo = float4(0.75f, 0.5f, 0.5f, 1.0f); // init as sligtly red object
  float4 textureColour = (float4)0; // object colour based only on texture
  //float3 lightDir;
  float lightIntensity = (float)0;
  float4 finalColour = (float4)0;

  float3 reflection = (float3)0;
  float4 specular = (float4)0;
  float4 finalSpec = (float4)0;


  // Set the default output colour to the ambient light value for all pixels.
  finalColour = ambientColour;

  // add texture color to albedo
  //albedo += shaderTexture.Sample(SampleType, input.tex);

  // Invert the light direction for calculations.
  //lightDir = -lightDirection;

  // Calculate the amount of light on this pixel.
  // saturate() clamps value between 0-1
  lightIntensity = saturate(dot(input.normal, -lightDirection));

  if (lightIntensity > 0.0f)
  {
    // Determine the final amount of diffuse color based on the diffuse color combined
    //with the light intensity.
    //commentiong out this line will remove the diffuse light
    finalColour += saturate(diffuseColour * lightIntensity);

    // Calculate reflection vector based on the light intensity, normal vector and light direction
    reflection = reflect(lightDirection, input.normal);

    // Determine the amount of specular light based on the reflection  vector,
    // viewing direction, and specular power.
    specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

    //sum up specular light
    finalSpec = specularColour * specular;
  }

  // Calculate the amount of light on this pixel.
  //lightIntensity = saturate(dot(input.normal, lightDir));

  if (hasTexture)
  {
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    finalColour *= textureColour;

  }


  // Multiply the albedo and the final diffuse color to get the final pixel color result.
  //finalColour *= albedo;

  // Add the specular component last to the output colour.
  finalColour = saturate(finalColour + finalSpec);

  return finalColour;
}

