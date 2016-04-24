
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
  float3 cameraPosition;
  float padding1;
  float4 cameraUp;
};


struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};

// pixel input type
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> spriteStream)
{
  float size = 1.0;
  OutputType output = (OutputType)0;
  float3 up = normalize(cameraUp.xyz);
  float3 direction = normalize(cameraPosition - input[0].position.xyz);
  float3 right = cross(direction, up);

  float3 offsetX = size * right;
  float3 offsetY = size * up;


  float4 g_positions[4] = { float4(-offsetX + offsetY, 1.0f),
                            float4(-offsetX - offsetY, 1.0f),
                            float4(offsetX + offsetY, 1.0f),
                            float4(offsetX - offsetY, 1.0f)
                          };

  //float4 g_positions[4] = { float4(-1.0f, 1.0f, 0.0f, 0.0f),
  //                          float4(-1.0f, -1.0f, 0.0f, 0.0f),
  //                          float4(1.0f, 1.0f, 0.0f, 0.0f),
  //                          float4(1.0f, -1.0f, 0.0f, 0.0f)
  //                        };

  // Change the position vector to be 4 units for proper matrix calculations.
  input[0].position.w = 1.0f;

  for (int i = 0; i < 4; i++)
  {
    // Move the vertex away from the point position
    output.position = input[0].position + g_positions[i];

    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input[0].tex;

    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

    spriteStream.Append(output);
  }


  spriteStream.RestartStrip();
}