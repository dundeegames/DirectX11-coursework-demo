////////////////////////////////////////////////////////////////////////////////
/**
* \file       ubertessell_vs.hlsl
* \brief      Uber Tessellation domain shader
*
* \details    After tessellation the domain shader processes the all the vertices
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



Texture2D heightMap : register(t0);
SamplerState SampleType : register(s0);


cbuffer MatrixBuffer : register(cb0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
  float3 cameraPosition;
  float padding;
};


struct ConstantOutputType
{
  float edges[4] : SV_TessFactor;
  float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
  float3 position : POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
};

struct OutputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
  float3 viewDirection : TEXCOORD1;
  float3 position3D : TEXCOORD2;
};

[domain("quad")]

OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
  //float displacement = 5.0f;
  float displacement = 0.0f;

  float3 vertexPosition, pos1, pos2;
  float3 vertexNormal, nor1, nor2;
  float2 texturePosition, tex1, tex2;  // texture coords

  OutputType output = (OutputType)0;
  float4 worldPosition = (float4)0;

  // Determine the position of the new vertex.
  // Invert the y and Z components of uvwCoord as these coords are generated in UV space
  // and therefore y is positive downward.
  // Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
  //vertexPosition = uvwCoord.x * patch[0].position + -uvwCoord.y * patch[1].position + -uvwCoord.z * patch[2].position;

  pos1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
  pos2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
  vertexPosition = lerp(pos1, pos2, uvwCoord.x);

  nor1 = lerp(patch[0].normal, patch[1].normal, 1 - uvwCoord.y);
  nor2 = lerp(patch[2].normal, patch[3].normal, 1 - uvwCoord.y);
  vertexNormal = lerp(nor1, nor2, uvwCoord.x);
  

  tex1 = lerp(patch[0].tex, patch[1].tex, 1 - uvwCoord.y);
  tex2 = lerp(patch[2].tex, patch[3].tex, 1 - uvwCoord.y);
  texturePosition = lerp(tex1, tex2, uvwCoord.x);

  //vertexPosition.y = heightMap.SampleLevel(SampleType, texturePosition, 0).x * displacement;


  // Calculate the position of the vertex in the world.
  worldPosition = mul(float4(vertexPosition, 1.0f), worldMatrix);

  // Determine the viewing direction based on the position of the camera
  // and the position of the vertex in the world.
  output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

  // Normalize the viewing direction vector.
  output.viewDirection = normalize(output.viewDirection);


  // Store the position of the vertex in the world.
  output.position3D = worldPosition.xyz;


  // Calculate the position of the new vertex against the world, view, and projection matrices.
  output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  // Store the texture coordinates for the pixel shader.
  output.tex = texturePosition;

  // Calculate the normal vector against the world matrix only.
  output.normal = (float3)( mul(float4(vertexNormal, 1.0f), worldMatrix) );

  // Normalize the normal vector.
  output.normal = normalize(output.normal);


  // Send the output into the pixel shader
  return output;
}

