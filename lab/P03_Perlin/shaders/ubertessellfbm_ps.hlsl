////////////////////////////////////////////////////////////////////////////////
/**
* \file       uber_ps.hlsl
* \brief      Uber Tessellation pixel shader
*
* \details    Calculate lighting for a MAX_LIGHTS (also texturing)
*
* \author     Jiri Klic
* \version    1.0
* \date       February 2016
* \pre
* \post
* \bug        No known bugs
* \warning
* \todo
*
* \copyright  University of Abertay - Dundee.2013.
*             Intellectual Property Policy.[online].Available from:
*             http://www.abertay.ac.uk/media/Intellectual-Property-Policy-v2-01.pdf
*             [Accessed 22 April 2015].
*
*///////////////////////////////////////////////////////////////////////////////




// DEFINES /////////////////////////////////////////////////////////////////////

#define MAX_LIGHTS 8
#define SWITCH_ON 1
#define SWITCH_OFF 0

#define SAND    2.0f
#define FOREST  3.5f
//#define ROCK    2.0f
#define SNOW    5.0f

// DATA ////////////////////////////////////////////////////////////////////////

static const float4 CLR_WHITE = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float4 CLR_GRAY = float4(0.3f, 0.3f, 0.3f, 1.0f);
static const float4 CLR_BLACK = float4(0.0f, 0.0f, 0.0f, 1.0f);
static const float4 CLR_GREEN = float4(0.0f, 0.25f, 0.0f, 1.0f);
static const float4 CLR_YELLOW = float4(0.5f, 0.5f, 0.0f, 1.0f);
static const float4 CLR_BLUE = float4(0.0f, 0.0f, 1.0f, 1.0f);


// SHADER //////////////////////////////////////////////////////////////////////


Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct LightData
{
  float4 ambient;
  float4 diffuse;
  float4 position;  // =1 directional light, <1 point light
  float4 specular;
  float specularPower;
  int isActive;
  int hasSpecularity;
  int padding;
};

cbuffer LightBuffer : register(cb0)
{
  LightData lightData[MAX_LIGHTS];
};


struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 position3D : TEXCOORD2;
};


float4 main(InputType input) : SV_TARGET
{
  float4 textureColour = (float4)0; // object colour based only on texture
  float3 lightDirection = (float3)0;
  float lightIntensity = (float)0;
  float4 finalColour = float4(0.0f, 0.0f, 0.0f, 1.0f);

  float3 reflection = (float3)0;
  float4 specularFX = (float4)0;
  float4 finalSpec = (float4)0;


  // Set the default output colour to the ambient light value for all pixels.
  for (int a = 0; a < MAX_LIGHTS; a++)
  {
    if (lightData[a].isActive == SWITCH_ON)
    {
      finalColour += lightData[a].ambient;
    }
  }
  finalColour = saturate(finalColour);


  //// Sample the pixel color from the texture using the sampler at this texture coordinate location.
  //textureColour = shaderTexture.Sample(SampleType, input.tex);

  // Determine pixel texture colour based on its y pos.
  if (input.position3D.y > SNOW)
  {
    textureColour = CLR_WHITE;
  }
  else if ((input.position3D.y < SNOW) && (input.position3D.y > FOREST))
  {
    textureColour = CLR_GRAY;
    //textureColour = lerp(yellowColour, greenColour, (position.y / (0.4f * displacement)));
  }
  else if ((input.position3D.y < FOREST) && (input.position3D.y > SAND))
  {
    textureColour = CLR_GREEN;
  }
  else
  {
    textureColour = CLR_YELLOW;
  }




  // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
  //finalColour.xyz *= textureColour.xyz;
  finalColour = saturate(finalColour * textureColour);


  //if (hasTexture)
  //{
  //  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  //  textureColour = shaderTexture.Sample(SampleType, input.tex);

  //  // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
  //  finalColour *= textureColour;
  //}

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    if (lightData[i].isActive == SWITCH_ON)
    {
      // Invert the light direction for calculations.
      lightDirection = normalize(input.position3D - lightData[i].position.xyz);

      // Calculate the amount of light on this pixel.
      // saturate() clamps value between 0-1
      lightIntensity = saturate(dot(input.normal, -lightDirection));

      if (lightIntensity > 0.0f)
      {
        // Determine the final amount of diffuse color based
        // on the diffuse color combinedwith the light intensity.
        // commentiong out this line will remove the diffuse light
        finalColour += saturate(lightData[i].diffuse * lightIntensity);

        if (lightData[i].hasSpecularity == SWITCH_ON)
        {
          // Calculate reflection vector based on the light intensity,
          // normal vector and light direction
          reflection = reflect(lightDirection, input.normal);

          // Determine the amount of specular light based on the reflection  vector,
          // viewing direction, and specular power.
          specularFX = pow(saturate(dot(reflection, input.viewDirection)), lightData[i].specularPower);

          //sum up specular light
          finalSpec = lightData[i].specular * specularFX;

          // Add the specular component last to the output colour.
          finalColour = saturate(finalColour + finalSpec);
        }

      }
    }
  }

  return finalColour;
}