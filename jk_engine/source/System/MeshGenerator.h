////////////////////////////////////////////////////////////////////////////////
/**
* \file       MeshGenerator.h
* \brief      MeshGenerator class - 
*
* \details    Generates Mesh primitives 
*
* \author     Jiri Klic
* \version    1.0
* \date       October 2015
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


#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include<Geometry/Mesh.h>
#include<vector>



enum PlaneType{ PLN_LEFT,
                PLN_RIGHT,
                PLN_FRONT,
                PLN_CENTER,
                PLN_BACK,
                PLN_TOP,
                PLN_BOTTOM,
                PLN_SURFACE
              };

// CLASS ///////////////////////////////////////////////////////////////////////

class MeshGenerator
{

public:
  MeshGenerator(ID3D11Device* device);
  ~MeshGenerator();

  Mesh* getTriangle(float size = 1.0f);
  Mesh* getQuad(float width = 1.0f, float height = 1.0f);

  Mesh* getPlane(float width = 1.0f, float height = 1.0f,
                  int subW = 1, int subH = 1, PlaneType type = PLN_CENTER);


private:
  Mesh* compileMesh();
  void makePlane(int subW, int subH, PlaneType type);


  ID3D11Device* m_Device;
  std::vector<VertexType> vertices;
  std::vector<unsigned long> indices;
};


#endif // MESH_GENERATOR_H




// 80 //////////////////////////////////////////////////////////////////////////