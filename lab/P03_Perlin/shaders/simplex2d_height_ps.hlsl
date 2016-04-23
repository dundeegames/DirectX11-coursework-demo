////////////////////////////////////////////////////////////////////////////////
/**
* \file       simplex2d_height_ps.hlsl
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

// 2D Simplex Noise
#define F2 0.366025403          // F2 = 0.5f *( sqrt(3.0f) - 1.0f)
#define G2 0.211324865          // G2 = (3.0f - sqrt(3.0f)) / 6.0f

#define SCALE_FACTOR2D 40.0f    // TODO: The scale factors are preliminary!


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


// FUNCTIONS ///////////////////////////////////////////////////////////////////

/**
* Helper function to compute gradients-dot-residualvectors (2D)
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

float  grad2(int hash, float x, float y)
{
  int h = hash & 7;               // Convert low 3 bits of hash code
  float u = h<4 ? x : y;          // into 8 simple gradient directions,
  float v = h<4 ? y : x;          // and compute the dot product with (x,y).
  return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
}

// -----------------------------------------------------------------------------

// 2D simplex noise
float snoise2(float x, float y)
{
  float n0, n1, n2; // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  float s = (x + y) * F2;       // Hairy factor for 2D
  float xs = x + s;
  float ys = y + s;
  int i = floor(xs);
  int j = floor(ys);

  float t = (float)(i + j) * G2;
  float X0 = i - t;             // Unskew the cell origin back to (x,y) space
  float Y0 = j - t;
  float x0 = x - X0;            // The x,y distances from the cell origin
  float y0 = y - Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  int i1, j1;         // Offsets for second (middle) corner of simplex in (i,j) coords

  if (x0 > y0)
  {
    i1 = 1; j1 = 0;             // lower triangle, XY order: (0,0)->(1,0)->(1,1)
  }
  else
  {
    i1 = 0; j1 = 1;             // upper triangle, YX order: (0,0)->(0,1)->(1,1)
  }

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6

  float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  float y1 = y0 - j1 + G2;
  float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
  float y2 = y0 - 1.0f + 2.0f * G2;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
  int ii = i & 0xff;
  int jj = j & 0xff;

  // Calculate the contribution from the three corners
  float t0 = 0.5f - x0*x0 - y0*y0;

  if (t0 < 0.0f)
  {
    n0 = 0.0f;
  }
  else
  {
    t0 *= t0;
    n0 = t0 * t0 * grad2(perm[ii + perm[jj]], x0, y0);
  }

  float t1 = 0.5f - x1*x1 - y1*y1;

  if (t1 < 0.0f)
  {
    n1 = 0.0f;
  }
  else
  {
    t1 *= t1;
    n1 = t1 * t1 * grad2(perm[ii + i1 + perm[jj + j1]], x1, y1);
  }

  float t2 = 0.5f - x2*x2 - y2*y2;

  if (t2 < 0.0f)
  {
    n2 = 0.0f;
  }
  else
  {
    t2 *= t2;
    n2 = t2 * t2 * grad2(perm[ii + 1 + perm[jj + 1]], x2, y2);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return SCALE_FACTOR2D * (n0 + n1 + n2); // TODO: The scale factor is preliminary!
}


// SHADER //////////////////////////////////////////////////////////////////////


cbuffer simplex2DBuffer : register(cb0)
{
  float frequency;
  float amplitude;
  float xOffset;
  float yOffset;
};


struct InputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
  float4 finalColor = (float4)0;
  float xCoord = (input.tex.x * frequency) + xOffset;
  float yCoord = (input.tex.y * frequency) + yOffset;

  // Calculate the height value using simplex 2d noise at this texture coordinate location.
  finalColor.w = (amplitude * snoise2(xCoord, yCoord));
    
  return finalColor;
}