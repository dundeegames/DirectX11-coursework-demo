////////////////////////////////////////////////////////////////////////////////
/**
* \file       PerlinNoise.h
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
* \copyright  http://www.fundza.com/c4serious/noise/perlin/index.html
*
* \copyright  University of Abertay - Dundee.2013.
*             Intellectual Property Policy.[online].Available from: 
*             http://www.abertay.ac.uk/media/Intellectual-Property-Policy-v2-01.pdf
*             [Accessed 22 April 2015].
*
*///////////////////////////////////////////////////////////////////////////////


#ifndef PERLINNOISE_H
#define PERLINNOISE_H


// INCLUDES ////////////////////////////////////////////////////////////////////






// CLASS ///////////////////////////////////////////////////////////////////////

class PerlinNoise
{

public:
  PerlinNoise();
  ~PerlinNoise();

  double noise(double x, double y, double z);
 

private:

  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
  

  int* p;  
  static const int permutation[];


};


#endif // PERLINNOISE_H




// 80 //////////////////////////////////////////////////////////////////////////