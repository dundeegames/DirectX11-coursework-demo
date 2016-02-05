////////////////////////////////////////////////////////////////////////////////
/**
* \file       QuadMesh.h
* \brief      QuadMesh class - 
*
* \details    Based on BaseMesh 
*
* \author     Jiri Klic
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


#ifndef QUADMESH_H
#define QUADMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"





// CLASS ///////////////////////////////////////////////////////////////////////

class QuadMesh : public BaseMesh
{

public:
  QuadMesh(ID3D11Device* device, WCHAR* textureFilename);
  ~QuadMesh();

  void setTexture(WCHAR* textureFilename);

protected:
  void InitBuffers(ID3D11Device* device);
  ID3D11Device* device_;

};


#endif // QUADMESH_H




// 80 //////////////////////////////////////////////////////////////////////////