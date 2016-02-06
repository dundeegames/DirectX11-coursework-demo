////////////////////////////////////////////////////////////////////////////////
/**
* \file       uber_vs.hlsl
* \brief      Uber vertex shader 
*
* \details    Standard issue vertex shader, apply matrices,
*             pass info to pixel shader 
*
* \author     Jiri Klic
* \version    1.0
* \date       December 2015
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

struct InputType
{
  float4 position : POSITION;
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

OutputType main(InputType input)
{
  OutputType output = (OutputType)0;
  float4 worldPosition = (float4)0;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;

  // Calculate the position of the vertex in the world.
  worldPosition = mul(input.position, worldMatrix);

  // Determine the viewing direction based on the position of the camera
  // and the position of the vertex in the world.
  output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

  // Normalize the viewing direction vector.
  output.viewDirection = normalize(output.viewDirection);


  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
    
  // Store the texture coordinates for the pixel shader.
  output.tex = input.tex;

  // Calculate the normal vector against the world matrix only.
  output.normal = mul(input.normal, (float3x3)worldMatrix);
	
  // Normalize the normal vector.
  output.normal = normalize(output.normal);

  // Calculate the position of the vertex in the world.
  output.position3D = mul(input.position, worldMatrix);

  return output;
}