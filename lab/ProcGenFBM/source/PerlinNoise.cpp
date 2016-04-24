////////////////////////////////////////////////////////////////////////////////
/**
* \file       PerlinNoise.cpp
* \brief      PerlinNoise class -
*
* \details    This code implements the algorithm Ken Perlin describe in a
*             corresponding SIGGRAPH 2002 paper. Translated from Java to C++
*             by Jiri Klic
*
* \author     Ken Perlin
* \author     Jiri Klic
* \version    2.0
* \date       February 2016
* \pre
* \post
* \bug        No known bugs
* \warning
* \todo
*
* \copyright  Reference implementation of improved noise - 2002 Ken Perlin
*
* \copyright  University of Abertay - Dundee.2013.
*             Intellectual Property Policy.[online].Available from:
*             http://www.abertay.ac.uk/media/Intellectual-Property-Policy-v2-01.pdf
*             [Accessed 22 April 2015].
*
*///////////////////////////////////////////////////////////////////////////////





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "PerlinNoise.h"

#include <math.h>
#include <stdio.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////

const int PerlinNoise::permutation[] = { 151, 160, 137, 91, 90, 15, 131, 13, 201,
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




PerlinNoise::PerlinNoise()
{
  p = new int[512];

  for (int i = 0; i < 256; i++)
  {
    p[256 + i] = p[i] = permutation[i];
  }

}


PerlinNoise::~PerlinNoise()
{

}


// -----------------------------------------------------------------------------

double PerlinNoise::fade(double t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

// -----------------------------------------------------------------------------

double PerlinNoise::lerp(double t, double a, double b)
{
  return a + t * (b - a);
}

// -----------------------------------------------------------------------------

double PerlinNoise::grad(int hash, double x, double y, double z)
{
  int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE

  double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
         v = h<4 ? y : h == 12 || h == 14 ? x : z;

  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);

}

// -----------------------------------------------------------------------------

double PerlinNoise::noise(double x, double y, double z)
{
  // FIND UNIT CUBE THAT CONTAINS POINT
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;
  int Z = (int)floor(z) & 255;


  // FIND RELATIVE X,Y,Z OF POINT IN CUBE
  x -= floor(x);
  y -= floor(y);
  z -= floor(z);


  // COMPUTE FADE CURVES FOR EACH OF X,Y,Z
  double u = fade(x);
  double v = fade(y);
  double w = fade(z);


  // HASH COORDINATES OF THE 8 CUBE CORNERS
  int A = p[X] + Y;
  int AA = p[A] + Z;
  int AB = p[A + 1] + Z;
  int B = p[X + 1] + Y;
  int BA = p[B] + Z;
  int BB = p[B + 1] + Z;


  // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE
  return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
              grad(p[BA], x - 1, y, z)),
              lerp(u, grad(p[AB], x, y - 1, z),
              grad(p[BB], x - 1, y - 1, z))),
              lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
              grad(p[BA + 1], x - 1, y, z - 1)),
              lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
              grad(p[BB + 1], x - 1, y - 1, z - 1))));


}

// -----------------------------------------------------------------------------






// 80 //////////////////////////////////////////////////////////////////////////