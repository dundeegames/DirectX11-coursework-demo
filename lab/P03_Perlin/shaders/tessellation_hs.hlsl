// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessellationBuffer : register(cb)
{
    float tessellationFactor;
    float3 cameraPosition;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 colour : COLOR;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
  ConstantOutputType output = (ConstantOutputType)0;

  // Set the tessellation factors for the three edges of the quad.
  for (int i = 0; i < 4; i++)
  {
    output.edges[i] = tessellationFactor;
  }

  // Set the tessellation factor for tessallating inside the triangle.
  output.inside[0] = tessellationFactor;
  output.inside[1] = tessellationFactor;

  return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input tex as the output tex.
    output.tex = patch[pointId].tex;

    // Set the input color as the output color.
    output.colour = patch[pointId].colour;

    return output;
}