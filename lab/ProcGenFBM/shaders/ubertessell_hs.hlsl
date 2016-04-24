////////////////////////////////////////////////////////////////////////////////
/**
* \file       ubertessell_vs.hlsl
* \brief      Uber Tessellation Hull Shader
*
* \details    Prepares control points for tessellation
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



cbuffer CameraBuffer : register(cb0)
{
  float3 cameraPosition;
  float tessellationFactor;    
};

struct InputType
{
  float3 position : POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
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
  float3 normal : NORMAL;
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
    output.normal = patch[pointId].normal;

    return output;
}