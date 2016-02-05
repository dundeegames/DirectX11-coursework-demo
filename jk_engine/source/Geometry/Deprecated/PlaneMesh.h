////////////////////////////////////////////////////////////////////////////////
/**
* \file       PlaneMesh.h
* \brief      PlaneMesh class -
*
* \details    Quad mesh made of many quads. Default is 100x100
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


#ifndef PLANEMESH_H
#define PLANEMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"





// CLASS ///////////////////////////////////////////////////////////////////////

class PlaneMesh : public BaseMesh
{

public:
  PlaneMesh(ID3D11Device* device, WCHAR* textureFilename, int resolution = 100);
  ~PlaneMesh();

protected:
  void InitBuffers(ID3D11Device* device);
  int m_resolution;

};


#endif // PLANEMESH_H




// 80 //////////////////////////////////////////////////////////////////////////