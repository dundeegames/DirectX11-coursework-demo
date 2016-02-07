// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

Texture2D heightMap : register(t0);
SamplerState SampleType : register(s0);


cbuffer MatrixBuffer : register(cb0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
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
  float4 colour : COLOR;
};

struct OutputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float4 colour : COLOR;
};

[domain("quad")]

OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
  float4 sampleColour;
  float4 whiteColour = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 grayColour = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 blackColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 greenColour = float4(0.0f, 0.25f, 0.0f, 1.0f);
    float4 yellowColour = float4(0.5f, 0.5f, 0.0f, 1.0f);
    float4 blueColour = float4(0.0f, 0.0f, 1.0f, 1.0f);
    float displacement = 5.0f;


  float3 vertexPosition, pos1, pos2;
  float2 texturePosition, tex1, tex2;  // texture coords
  OutputType output;

  // Determine the position of the new vertex.
  // Invert the y and Z components of uvwCoord as these coords are generated in UV space
  // and therefore y is positive downward.
  // Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
  //vertexPosition = uvwCoord.x * patch[0].position + -uvwCoord.y * patch[1].position + -uvwCoord.z * patch[2].position;

  pos1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
  pos2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
  vertexPosition = lerp(pos1, pos2, uvwCoord.x);

  tex1 = lerp(patch[0].tex, patch[1].tex, 1 - uvwCoord.y);
  tex2 = lerp(patch[2].tex, patch[3].tex, 1 - uvwCoord.y);
  texturePosition = lerp(tex1, tex2, uvwCoord.x);

  //pos1 = lerp(patch[0].position, patch[1].position, uvwCoord.x);
  //pos2 = lerp(patch[2].position, patch[3].position, uvwCoord.x);
  //vertexPosition = lerp(pos1, pos2, uvwCoord.y);

  //tex1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.x);
  //tex2 = lerp(patch[2].tex, patch[3].tex, uvwCoord.x);
  //texturePosition = lerp(tex1, tex2, uvwCoord.y);


  vertexPosition.y = heightMap.SampleLevel(SampleType, texturePosition, 0).x * displacement;

  if (vertexPosition.y > (0.7f * displacement))
  {
    output.colour = whiteColour;
  }
  else if (vertexPosition.y < (0.3f * displacement))
  {
    output.colour = lerp(blueColour, yellowColour, (vertexPosition.y / (0.3f * displacement) ));
  }
  else
  {
    output.colour = lerp(yellowColour, greenColour, (vertexPosition.y / (0.4f * displacement) ));
  }





  // Calculate the position of the new vertex against the world, view, and projection matrices.
  output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  output.tex = texturePosition;

  // Send the input color into the pixel shader.
  //output.colour = patch[0].colour;

  return output;
}

