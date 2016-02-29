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

static int permutation[] = { 151, 160, 137, 91, 90, 15, 131, 13, 201,
95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10,
23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35,
11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211,
133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188,
159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202,
38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189,
28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155,
167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185,
112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184,
84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67,
29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };


//void initNoise(int* p)
//{
//  // init noise
//  p = new int[512];
//
//  for (int i = 0; i < 256; i++)
//  {
//    p[256 + i] = p[i] = permutation[i];
//  }
//}

// -----------------------------------------------------------------------------

float fade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

// -----------------------------------------------------------------------------

//float lerp(float t, float a, float b)
//{
//  return a + t * (b - a);
//}

// -----------------------------------------------------------------------------

//float grad(int hash, float x, float y, float z)
//{
//  int h = hash & 15;                     // CONVERT LO 4 BITS OF HASH CODE
//
//  float u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
//    v = h<4 ? y : h == 12 || h == 14 ? x : z;
//
//  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
//
//}

// -----------------------------------------------------------------------------









// -----------------------------------------------------------------------------


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
  output.normal = mul(vertexNormal, (float3x3)worldMatrix);

  // Normalize the normal vector.
  output.normal = normalize(output.normal);


  // Send the output into the pixel shader
  return output;
}

