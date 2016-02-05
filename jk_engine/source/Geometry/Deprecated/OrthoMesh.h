////////////////////////////////////////////////////////////////////////////////
/**
* \file       OrthoMesh.h
* \brief      OrthoMesh class -
*
* \details    Orthographic mesh for post processing,
*             should render a quad to match window size
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


#ifndef ORTHOMESH_H
#define ORTHOMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "basemesh.h"

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class OrthoMesh : public BaseMesh
{

public:
  OrthoMesh(ID3D11Device* device, int width, int height, int xPosition = 0, int yPosition = 0);
  ~OrthoMesh();

protected:
  void InitBuffers(ID3D11Device*);
  int m_width, m_height, m_xPosition, m_yPosition;

};


#endif // ORTHOMESH_H




// 80 //////////////////////////////////////////////////////////////////////////