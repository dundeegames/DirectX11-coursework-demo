////////////////////////////////////////////////////////////////////////////////
/**
* \file       TriangleMesh.h
* \brief      TriangleMesh class -
*
* \details    Simple single triangle mesh (for example purposes)
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


#ifndef TRIMESH_H
#define TRIMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class TriangleMesh : public BaseMesh
{

public:
  TriangleMesh(ID3D11Device* device, WCHAR* textureFilename);
  ~TriangleMesh();

protected:
  void InitBuffers(ID3D11Device* device);

};


#endif // TEMP_H




// 80 //////////////////////////////////////////////////////////////////////////