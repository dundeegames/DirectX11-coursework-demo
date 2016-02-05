////////////////////////////////////////////////////////////////////////////////
/**
* \file       PointMesh.h
* \brief      PointMesh class -
*
* \details    Tessellation Mesh, point list for tessellation
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


#ifndef POINTMESH_H
#define POINTMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class PointMesh : public BaseMesh
{

public:
  PointMesh(ID3D11Device* device, WCHAR* textureFilename);
  ~PointMesh();

  void SendData(ID3D11DeviceContext*);

protected:
  void InitBuffers(ID3D11Device* device);

};


#endif // POINTMESH_H




// 80 //////////////////////////////////////////////////////////////////////////