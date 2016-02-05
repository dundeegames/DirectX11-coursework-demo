// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
  float3 cameraPosition;
  float time;
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
};

OutputType main(InputType input)
{
  OutputType output = (OutputType)0;
  float4 worldPosition = (float4)0;
  float height = 1.0f;
  float frequency = 25.0f;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;

  //offset position based on sine wave
  input.position.z = height * sin(frequency * (input.position.x + time));

  //modify normals
  input.normal.x = 1 - cos(frequency * (input.position.x + time));
  input.normal.z = abs(cos(frequency * (input.position.x + time)));




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

  return output;
}