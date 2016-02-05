////////////////////////////////////////////////////////////////////////////////
/**
* \file       SphereMesh.h
* \brief      SphereMesh class -
*
* \details    Generates a sphere
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


#ifndef SPHEREMESH_H
#define SPHEREMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class SphereMesh : public BaseMesh
{

public:
  SphereMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 20);
  ~SphereMesh();

protected:
  void InitBuffers(ID3D11Device* device);
  int m_resolution;

};


#endif // SPHEREMESH_H




// 80 //////////////////////////////////////////////////////////////////////////