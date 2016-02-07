////////////////////////////////////////////////////////////////////////////////
/**
* \file       ubertessell_vs.hlsl
* \brief      Uber Tessellation vertex shader
*
* \details    Doesn't do much, could manipulate the control points
*             Pass forward data, strip out some values not required for example.
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
    float3 normal : NORMAL;
};

OutputType main(InputType input)
{
  OutputType output = (OutputType)0;

	// Pass the vertex position and normal into the hull shader.
  output.position = input.position;
  output.normal = input.normal;

  output.tex = mul(input.tex, TextureMatrix);
  //output.tex = input.tex;

    
  return output;
}
