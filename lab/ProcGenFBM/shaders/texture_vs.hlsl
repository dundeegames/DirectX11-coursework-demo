// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(cb0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
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
};

OutputType main(InputType input)
{
	OutputType output;
  //input.position.xyz *= 2.0f;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;

  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  // Store the texture coordinates for the pixel shader.
  //output.tex = input.tex * 2.0f + 0.5f;
  output.tex = input.tex;

  // Calculate the normal vector against the world matrix only.
  output.normal = (float3)(mul(float4(input.normal, 1.0f), worldMatrix));
	
  // Normalize the normal vector.
  output.normal = normalize(output.normal);
    
  return output;
}