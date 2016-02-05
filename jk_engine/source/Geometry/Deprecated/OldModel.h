////////////////////////////////////////////////////////////////////////////////
/**
* \file       OldModel.h
* \brief      Model class -
*
* \details    Loads a .obj and creates a mesh object from the data
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


#ifndef OLDMODEL_H
#define OLDMODEL_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseMesh.h"
#include <System/TokenStream.h>
#include <vector>

using namespace DirectX;

// CLASS ///////////////////////////////////////////////////////////////////////

class OldModel : public BaseMesh
{
  struct ModelType
  {
    float x, y, z;
    float tu, tv;
    float nx, ny, nz;
  };

public:
  OldModel(ID3D11Device* device, WCHAR* textureFilename, WCHAR* filename);
  ~OldModel();

protected:
  void InitBuffers(ID3D11Device* device);
  void LoadModel(WCHAR* filename);

  ModelType* m_model;

};


#endif // OLDMODEL_H




// 80 //////////////////////////////////////////////////////////////////////////