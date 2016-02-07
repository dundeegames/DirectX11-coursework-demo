////////////////////////////////////////////////////////////////////////////////
/**
* \file       TerrainMesh.h
* \brief      TerrainMesh class -
*
* \details    TerrainMesh Mesh, point list for tessellation
*
* \author     Jiri Klic
* \version    1.0
* \date       February 2016
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


#ifndef TESSELLATIONMESH_H
#define TESSELLATIONMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "Deprecated/BaseMesh.h"
#include <vector>

using namespace DirectX;



// CLASS ///////////////////////////////////////////////////////////////////////

class TerrainMesh : public BaseMesh
{

public:
  TerrainMesh(ID3D11Device* device, WCHAR* textureFilename,
              float width = 50.0f, float height = 50.0f, int subW = 1, int subH = 1);

  ~TerrainMesh();

  void SendData(ID3D11DeviceContext*);

private:
  void GeneratePatch(float width, float height, int subW, int subH);
  void InitBuffers(ID3D11Device* device);

private:


  std::vector<VertexType> m_Vertices;
  std::vector<unsigned long> m_Indices;


};


#endif // TESSELLATIONMESH_H




// 80 //////////////////////////////////////////////////////////////////////////