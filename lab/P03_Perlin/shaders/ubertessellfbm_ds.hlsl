////////////////////////////////////////////////////////////////////////////////
/**
* \file       ubertessellsimplex_ds.hlsl
* \brief      Uber Tessellation domain shader with Perlin Simplex Noise
*
* \details    This implementation is "Simplex Noise" as presented by Ken Perlin
*             at a relatively obscure and not often cited course session
*             "Real-Time Shading" at Siggraph 2001 (before real time shading actually took on),
*             under the title "hardware noise". The 3D function is numerically
*             equivalent to his Java reference code available in the PDF course notes,
*             although I re-implemented it from scratch to get more readable code.
*             The 1D, 2D and 4D cases were implemented from scratch by me from Ken Perlin's text.
*
*             Gustavson, S. 2015. DSOnoises, a set of useful noise functions for SL. (Version 1.0).
*             [source code]. Available from: http://staffwww.itn.liu.se/~stegu/simplexnoise/DSOnoises.html
*             [Accessed 5 April 2016].
*
*             Note: Ported from C to HLSL domain shader by Jiri Klic. 
*
* \author     Stefan Gustavson (stegu@itn.liu.se), 2003-2005
* \author     Jiri Klic
* \version    2.0
* \date       April 2016
* \pre
* \post
* \bug        No known bugs
* \warning
* \todo
*
* \copyright  [Stefan Gustavson] This code was GPL licensed until February 2011.
*             As the original author of this code, I hereby release it into the
*             public domain. Please feel free to use it for whatever you want.
*             Credit is appreciated where appropriate, and I also appreciate being
*             told where this code finds any use, but you may do as you like.
*
* \copyright  University of Abertay - Dundee.2016.
*             Intellectual Property Policy.[online].Available from:
*             http://www.abertay.ac.uk/media/intellectual%20property%20policy%20january%202016.pdf
*             [Accessed 19 March 2016].
*
*///////////////////////////////////////////////////////////////////////////////



// DEFINES /////////////////////////////////////////////////////////////////////

// Simple skewing factors for the 3D case
#define F3 0.333333333          // F3 = 1.0f / 3.0f 
#define G3 0.166666667          // G3 = 1.0f / 6.0f

// The skewing and unskewing factors are hairy again for the 4D case
#define F4 0.309016994          // F4 = (sqrt(5.0f) - 1.0f) / 4.0f
#define G4 0.138196601          // G4 = (5.0f - sqrt(5.0f)) / 20.0f

#define SCALE_FACTOR3D 32.0f
#define SCALE_FACTOR4D 27.0f

// DATA ////////////////////////////////////////////////////////////////////////

/**
* Permutation table. This is just a random jumble of all numbers 0-255,
* repeated twice to avoid wrapping the index at 255 for each lookup.
* This needs to be exactly the same for all instances on all platforms,
* so it's easiest to just keep it as static explicit data.
* This also removes the need for any initialisation of this class.
*
* Note that making this an int[] instead of a char[] might make the
* code run faster on platforms with a high penalty for unaligned single
* byte addressing. Intel x86 is generally single-byte-friendly, but
* some other CPUs are faster with 4-aligned reads.
* However, a char[] is smaller, which avoids cache trashing, and that
* is probably the most important aspect on most architectures.
* This array is accessed a *lot* by the noise functions.
* A vector-valued noise over 3D accesses it 96 times, and a
* float-valued 4D noise 64 times. We want this to fit in the cache!
*/
static const int perm[512] = { 151, 160, 137, 91, 90, 15,
  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
  151, 160, 137, 91, 90, 15,
  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

// -----------------------------------------------------------------------------

// A lookup table to traverse the simplex around a given point in 4D.
// Details can be found where this table is used, in the 4D noise method.
/* TODO: This should not be required, backport it from Bill's GLSL code! */
static const int simplex[64][4] = {
  { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 }, { 0, 2, 3, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 2, 3, 0 },
  { 0, 2, 1, 3 }, { 0, 0, 0, 0 }, { 0, 3, 1, 2 }, { 0, 3, 2, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 3, 2, 0 },
  { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
  { 1, 2, 0, 3 }, { 0, 0, 0, 0 }, { 1, 3, 0, 2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 2, 3, 0, 1 }, { 2, 3, 1, 0 },
  { 1, 0, 2, 3 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 2, 0, 3, 1 }, { 0, 0, 0, 0 }, { 2, 1, 3, 0 },
  { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
  { 2, 0, 1, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 3, 0, 1, 2 }, { 3, 0, 2, 1 }, { 0, 0, 0, 0 }, { 3, 1, 2, 0 },
  { 2, 1, 0, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 3, 1, 0, 2 }, { 0, 0, 0, 0 }, { 3, 2, 0, 1 }, { 3, 2, 1, 0 } };

// -----------------------------------------------------------------------------


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


// FUNCTIONS ///////////////////////////////////////////////////////////////////

/**
* Helper functions to compute gradients-dot-residualvectors (1D to 4D)
* Note that these generate gradients of more than unit length. To make
* a close match with the value range of classic Perlin noise, the final
* noise values need to be rescaled to fit nicely within [-1,1].
* (The simplex noise functions as such also have different scaling.)
* Note also that these noise functions are the most practical and useful
* signed version of Perlin noise. To return values according to the
* RenderMan specification from the SL noise() and pnoise() functions,
* the noise values need to be scaled and offset to [0,1], like this:
* float SLnoise = (noise(x,y,z) + 1.0) * 0.5;
*/

float  grad3(int hash, float x, float y, float z)
{
  int h = hash & 15;              // Convert low 4 bits of hash code into 12 simple
  float u = h<8 ? x : y;          // gradient directions, and compute dot product.
  float v = h<4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

float  grad4(int hash, float x, float y, float z, float t)
{
  int h = hash & 31;              // Convert low 5 bits of hash code into 32 simple
  float u = h<24 ? x : y;         // gradient directions, and compute dot product.
  float v = h<16 ? y : z;
  float w = h<8 ? z : t;
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v) + ((h & 4) ? -w : w);
}

// -----------------------------------------------------------------------------

// 3D simplex noise
float snoise3(float x, float y, float z)
{
  float n0, n1, n2, n3; // Noise contributions from the four corners

  // Skew the input space to determine which simplex cell we're in
  float s = (x + y + z)*F3; // Very nice and simple skew factor for 3D
  float xs = x + s;
  float ys = y + s;
  float zs = z + s;
  int i = floor(xs);
  int j = floor(ys);
  int k = floor(zs);

  float t = (float)(i + j + k)*G3;
  float X0 = i - t; // Unskew the cell origin back to (x,y,z) space
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = x - X0; // The x,y,z distances from the cell origin
  float y0 = y - Y0;
  float z0 = z - Z0;

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
  // Determine which simplex we are in.
  int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
  int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

  /* This code would benefit from a backport from the GLSL version! */
  if (x0 >= y0)
  {
    if (y0 >= z0)
    {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;     // X Y Z order
    }
    else if (x0 >= z0)
    {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;     // X Z Y order
    }
    else
    {
      i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;     // Z X Y order
    }
  }
  else
  {
    // x0<y0
    if (y0 < z0)
    {
      i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;     // Z Y X order
    }
    else if (x0 < z0)
    {
      i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;     // Y Z X order
    }
    else
    {
      i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;     // Y X Z order
    }
  }

  // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
  // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
  // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
  // c = 1/6.

  float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2 = x0 - i2 + 2.0f*G3; // Offsets for third corner in (x,y,z) coords
  float y2 = y0 - j2 + 2.0f*G3;
  float z2 = z0 - k2 + 2.0f*G3;
  float x3 = x0 - 1.0f + 3.0f*G3; // Offsets for last corner in (x,y,z) coords
  float y3 = y0 - 1.0f + 3.0f*G3;
  float z3 = z0 - 1.0f + 3.0f*G3;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
  int ii = i & 0xff;
  int jj = j & 0xff;
  int kk = k & 0xff;

  // Calculate the contribution from the four corners
  float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
  if (t0 < 0.0f)
  {
    n0 = 0.0f;
  }
  else
  {
    t0 *= t0;
    n0 = t0 * t0 * grad3(perm[ii + perm[jj + perm[kk]]], x0, y0, z0);
  }

  float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
  if (t1 < 0.0f)
  {
    n1 = 0.0f;
  }
  else
  {
    t1 *= t1;
    n1 = t1 * t1 * grad3(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], x1, y1, z1);
  }

  float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
  if (t2 < 0.0f)
  {
    n2 = 0.0f;
  }
  else
  {
    t2 *= t2;
    n2 = t2 * t2 * grad3(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], x2, y2, z2);
  }

  float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
  if (t3 < 0.0f)
  {
    n3 = 0.0f;
  }
  else
  {
    t3 *= t3;
    n3 = t3 * t3 * grad3(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], x3, y3, z3);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to stay just inside [-1,1]
  return SCALE_FACTOR3D * (n0 + n1 + n2 + n3);
}

// -----------------------------------------------------------------------------

// 4D simplex noise
float snoise4(float x, float y, float z, float w)
{
  float n0, n1, n2, n3, n4; // Noise contributions from the five corners

  // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
  float s = (x + y + z + w) * F4; // Factor for 4D skewing
  float xs = x + s;
  float ys = y + s;
  float zs = z + s;
  float ws = w + s;
  int i = floor(xs);
  int j = floor(ys);
  int k = floor(zs);
  int l = floor(ws);

  float t = (i + j + k + l) * G4; // Factor for 4D unskewing
  float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
  float Y0 = j - t;
  float Z0 = k - t;
  float W0 = l - t;

  float x0 = x - X0;  // The x,y,z,w distances from the cell origin
  float y0 = y - Y0;
  float z0 = z - Z0;
  float w0 = w - W0;

  // For the 4D case, the simplex is a 4D shape I won't even try to describe.
  // To find out which of the 24 possible simplices we're in, we need to
  // determine the magnitude ordering of x0, y0, z0 and w0.
  // The method below is a good way of finding the ordering of x,y,z,w and
  // then find the correct traversal order for the simplex we’re in.
  // First, six pair-wise comparisons are performed between each possible pair
  // of the four coordinates, and the results are used to add up binary bits
  // for an integer index.
  int c1 = (x0 > y0) ? 32 : 0;
  int c2 = (x0 > z0) ? 16 : 0;
  int c3 = (y0 > z0) ? 8 : 0;
  int c4 = (x0 > w0) ? 4 : 0;
  int c5 = (y0 > w0) ? 2 : 0;
  int c6 = (z0 > w0) ? 1 : 0;
  int c = c1 + c2 + c3 + c4 + c5 + c6;

  int i1, j1, k1, l1; // The integer offsets for the second simplex corner
  int i2, j2, k2, l2; // The integer offsets for the third simplex corner
  int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

  // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
  // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
  // impossible. Only the 24 indices which have non-zero entries make any sense.
  // We use a thresholding to set the coordinates in turn from the largest magnitude.
  // The number 3 in the "simplex" array is at the position of the largest coordinate.
  i1 = simplex[c][0] >= 3 ? 1 : 0;
  j1 = simplex[c][1] >= 3 ? 1 : 0;
  k1 = simplex[c][2] >= 3 ? 1 : 0;
  l1 = simplex[c][3] >= 3 ? 1 : 0;

  // The number 2 in the "simplex" array is at the second largest coordinate.
  i2 = simplex[c][0] >= 2 ? 1 : 0;
  j2 = simplex[c][1] >= 2 ? 1 : 0;
  k2 = simplex[c][2] >= 2 ? 1 : 0;
  l2 = simplex[c][3] >= 2 ? 1 : 0;

  // The number 1 in the "simplex" array is at the second smallest coordinate.
  i3 = simplex[c][0] >= 1 ? 1 : 0;
  j3 = simplex[c][1] >= 1 ? 1 : 0;
  k3 = simplex[c][2] >= 1 ? 1 : 0;
  l3 = simplex[c][3] >= 1 ? 1 : 0;

  // The fifth corner has all coordinate offsets = 1, so no need to look that up.
  float x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
  float y1 = y0 - j1 + G4;
  float z1 = z0 - k1 + G4;
  float w1 = w0 - l1 + G4;
  float x2 = x0 - i2 + 2.0f*G4; // Offsets for third corner in (x,y,z,w) coords
  float y2 = y0 - j2 + 2.0f*G4;
  float z2 = z0 - k2 + 2.0f*G4;
  float w2 = w0 - l2 + 2.0f*G4;
  float x3 = x0 - i3 + 3.0f*G4; // Offsets for fourth corner in (x,y,z,w) coords
  float y3 = y0 - j3 + 3.0f*G4;
  float z3 = z0 - k3 + 3.0f*G4;
  float w3 = w0 - l3 + 3.0f*G4;
  float x4 = x0 - 1.0f + 4.0f*G4; // Offsets for last corner in (x,y,z,w) coords
  float y4 = y0 - 1.0f + 4.0f*G4;
  float z4 = z0 - 1.0f + 4.0f*G4;
  float w4 = w0 - 1.0f + 4.0f*G4;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
  int ii = i & 0xff;
  int jj = j & 0xff;
  int kk = k & 0xff;
  int ll = l & 0xff;

  // Calculate the contribution from the five corners
  float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0 - w0*w0;
  if (t0 < 0.0f)
  {
    n0 = 0.0f;
  }
  else
  {
    t0 *= t0;
    n0 = t0 * t0 * grad4(perm[ii + perm[jj + perm[kk + perm[ll]]]], x0, y0, z0, w0);
  }

  float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1 - w1*w1;
  if (t1 < 0.0f)
  {
    n1 = 0.0f;
  }
  else
  {
    t1 *= t1;
    n1 = t1 * t1 * grad4(perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]], x1, y1, z1, w1);
  }

  float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2 - w2*w2;
  if (t2 < 0.0f)
  {
    n2 = 0.0f;
  }
  else
  {
    t2 *= t2;
    n2 = t2 * t2 * grad4(perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]], x2, y2, z2, w2);
  }

  float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3 - w3*w3;
  if (t3 < 0.0f)
  {
    n3 = 0.0f;
  }
  else
  {
    t3 *= t3;
    n3 = t3 * t3 * grad4(perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]], x3, y3, z3, w3);
  }

  float t4 = 0.6f - x4*x4 - y4*y4 - z4*z4 - w4*w4;
  if (t4 < 0.0f)
  {
    n4 = 0.0f;
  }
  else
  {
    t4 *= t4;
    n4 = t4 * t4 * grad4(perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]], x4, y4, z4, w4);
  }

  // Sum up and scale the result to cover the range [-1,1]
  return (n0 + n1 + n2 + n3 + n4);
}


// -----------------------------------------------------------------------------


/**
* \brief      Ridget multifractal terrain model
*
* \details    Based on Texturing & Modeling procedural approach 3rd Ed., Chapter 16 
* \param      float H = 1.0, offset = 1.0, gain 2.0
* \return     float height
*/
float ridgetMultifractal(float3 inputPoint, float H, float lacunarity, float offset, float gain)
{
  const int OCTAVES = 8;

  float3 position = inputPoint;
  float frequency = 1.0f;
  float result = 0.0f;
  float signal = 0.0f;
  float weight = 0.0f;
  
  //float exponent_array[OCTAVES + 1];

  //// precompute spectral weights
  //for (int i = 0; i <= OCTAVES; i++)
  //{
  //  exponent_array[i] = pow(frequency, -H);
  //  frequency *= lacunarity;
  //}


  for (int i = 0; i < OCTAVES; i++)
  {
    signal = (snoise3(position.x, position.y, position.z) * pow(lacunarity, -H * i));

    signal = abs(signal);

    result += signal;

    position.x *= lacunarity;
    position.y *= lacunarity;
    position.z *= lacunarity;

  }


  //// get first octave
  //signal = snoise3(position.x, position.y, position.z);

  //// get abs value of signal (this creates the ridges)
  //signal = abs(signal);

  ////// invert and translate (offset should be ~= 1.0)
  ////signal = offset - signal;

  //// square the signal to sharpen the ridges
  //signal *= signal;

  //// assign initial values
  //result = signal;
  //weight = 1.0f;








  //// get first octave
  //signal = snoise3(position.x, position.y, position.z);

  //// get abs value of signal (this creates the ridges)
  //signal = abs(signal);

  ////// invert and translate (offset should be ~= 1.0)
  ////signal = offset - signal;

  //// square the signal to sharpen the ridges
  //signal *= signal;

  //// assign initial values
  //result = signal;
  //weight = 1.0f;

  //for (int j = 0; j < OCTAVES; j++)
  //{
  //  // increase the frequency
  //  position.x *= lacunarity;
  //  position.y *= lacunarity;
  //  position.z *= lacunarity;

  //  // weight successive contributions by previous signal
  //  weight = signal * gain;
  //  weight = clamp(weight, 0, 1);

  //  signal = snoise3(position.x, position.y, position.z);
  //  signal = abs(signal);
  //  signal = offset - signal;
  //  signal *= signal;

  //  // weight the contribution
  //  signal *= weight;
  //  result += signal * exponent_array[j];
  //}

  return result;
}



// SHADER //////////////////////////////////////////////////////////////////////

[domain("quad")]

OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
  int i = 0;
  float displacement = 0.0f;
  float3 vertexNormal = float3(0.0f, 0.0f, 0.0f);
  float3 tempNormal;

  float3 nor1, nor2;
  float3 vertexPosition, pos1, pos2;
  
  float3 noisePos[5];
  float3 noiseTan[4];

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

  tex1 = lerp(patch[0].tex, patch[1].tex, 1 - uvwCoord.y);
  tex2 = lerp(patch[2].tex, patch[3].tex, 1 - uvwCoord.y);
  texturePosition = lerp(tex1, tex2, uvwCoord.x);


  displacement = ridgetMultifractal(vertexPosition.xyz, 1.5f, 1.5f, 1.0f, 2.0f);
  //displacement *= 0.0001f;
  //displacement -= 9.0f;
  //displacement -= 97655.0f;
  //displacement = clamp(displacement, -1.0f, 3.0f);
  vertexPosition.y += displacement;



  nor1 = lerp(patch[0].normal, patch[1].normal, 1 - uvwCoord.y);
  nor2 = lerp(patch[2].normal, patch[3].normal, 1 - uvwCoord.y);
  vertexNormal = lerp(nor1, nor2, uvwCoord.x);
  vertexNormal.y += displacement;

  

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
  output.normal = output.normal = (float3)(mul(float4(vertexNormal, 1.0f), worldMatrix));

  // Normalize the normal vector.
  output.normal = normalize(output.normal);


  // Send the output into the pixel shader
  return output;
}

