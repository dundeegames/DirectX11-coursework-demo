////////////////////////////////////////////////////////////////////////////////
/**
* \file       CubeMesh.h
* \brief      CubeMesh class -
*
* \details    Generates a cube-sphere
*
* \author     Paul Robertson
* \version    1.0
* \date       September 2015
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


#ifndef CUBEMESH_H
#define CUBEMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class CubeMesh : public BaseMesh
{

public:
  CubeMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 20);
  ~CubeMesh();

protected:
  void InitBuffers(ID3D11Device* device);
  int m_resolution;

};


#endif // CUBEMESH_H




// 80 //////////////////////////////////////////////////////////////////////////