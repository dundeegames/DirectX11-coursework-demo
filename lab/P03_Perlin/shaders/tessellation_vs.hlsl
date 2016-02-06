// Tessellation vertex shader.
// Doesn't do much, could manipulate the control points
// Pass forward data, strip out some values not required for example.

cbuffer TextureMatrixBuffer : register(cb0)
{
  matrix TextureMatrix;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 colour : COLOR;
};

OutputType main(InputType input)
{
  OutputType output = (OutputType)0;

	// Pass the vertex position into the hull shader.
  output.position = input.position;

  output.tex = mul(input.tex, TextureMatrix);
  //output.tex = input.tex;


  // Pass the input color into the hull shader.
  output.colour = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    return output;
}
