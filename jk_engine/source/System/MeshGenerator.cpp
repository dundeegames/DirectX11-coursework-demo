////////////////////////////////////////////////////////////////////////////////
/**
* \file       MeshGenerator.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include"MeshGenerator.h"

#define LEFT_OFFSET -0.5f
#define TOP_OFFSET 0.5f

// FUNCTIONS ///////////////////////////////////////////////////////////////////


MeshGenerator::MeshGenerator(ID3D11Device* device) : m_Device(device)
{

}


MeshGenerator::~MeshGenerator()
{

}


// -----------------------------------------------------------------------------

Mesh* MeshGenerator::getTriangle(float size)
{
  // Load the vertex array with data.
  vertices.push_back({ XMFLOAT3(0.0f, 0.5f, 0.0f),
                       XMFLOAT2(0.0f, 0.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Top

  vertices.push_back({ XMFLOAT3(-0.5f, -0.5f, 0.0f),
                       XMFLOAT2(0.0f, 1.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Bottom left

  vertices.push_back({ XMFLOAT3(0.5f, -0.5f, 0.0f),
                       XMFLOAT2(1.0f, 1.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Bottom right


  // Load the index array with data.
  indices.push_back(0); // Top left
  indices.push_back(1); // Bottom left
  indices.push_back(2); // Bottom right

  Mesh* mesh = compileMesh();
  mesh->setScale(size, size, 1.0f);

  return mesh;
}

// -----------------------------------------------------------------------------

Mesh* MeshGenerator::getQuad(float width, float height)
{
  // Load the vertex array with data.
  vertices.push_back({ XMFLOAT3(-0.5f, 0.5f, 0.0f),
                       XMFLOAT2(0.0f, 0.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Top left

  vertices.push_back({ XMFLOAT3(0.5f, 0.5f, 0.0f),
                       XMFLOAT2(1.0f, 0.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Top right

  vertices.push_back({ XMFLOAT3(-0.5f, -0.5f, 0.0f),
                       XMFLOAT2(0.0f, 1.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Bottom left

  vertices.push_back({ XMFLOAT3(0.5f, -0.5f, 0.0f),
                       XMFLOAT2(1.0f, 1.0f),
                       XMFLOAT3(0.0f, 0.0f, -1.0f) });  // Bottom right

  
  // Load the index array with data.
  indices.push_back(0); // Top left
  indices.push_back(2); // Bottom left
  indices.push_back(3); // Bottom right
  indices.push_back(3); // Bottom right
  indices.push_back(1); // Top right
  indices.push_back(0); // Top left
  

  Mesh* mesh = compileMesh();
  mesh->setScale(width, height, 1.0f);

  return mesh;
}

// -----------------------------------------------------------------------------

Mesh* MeshGenerator::getPlane(float width, float height, int subW, int subH, PlaneType type)
{
  //makePlane(subX, subY, type);

  float dW = (1.0f / (float)subW);
  float dH = (1.0f / (float)subH);
  int row = (subW + 1);
  int column = (subH + 1);

  XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
  XMFLOAT2 UV = XMFLOAT2(0.0f, 0.0f);
  XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, -1.0f);  // Default

  for (int j = 0; j < column; j++)
  {
    for (int i = 0; i < row; i++)
    {
      position.x = LEFT_OFFSET + (i * dW);
      position.y = TOP_OFFSET - (j * dH);
      UV.x = (i * dW);
      UV.y = (j * dH);

      // Load the vertex array with data.
      vertices.push_back({ position, UV, normal });
    }
  }


  for (int j = 0; j < subH; j++)
  {
    for (int i = 0; i < subW; i++)
    {
      // Load the index array with data.
      indices.push_back((j*row) + i);             // Top left
      indices.push_back(((j + 1)*row) + i);       // Bottom left
      indices.push_back(((j + 1)*row) + (i + 1)); // Bottom right
      indices.push_back(((j + 1)*row) + (i + 1)); // Bottom right
      indices.push_back((j*row) + (i + 1));       // Top right
      indices.push_back((j*row) + i);             // Top left
    }
  }



  Mesh* mesh = compileMesh();
  mesh->setScale(width, height, 1.0f);

  return mesh;
}

// -----------------------------------------------------------------------------

Mesh* MeshGenerator::compileMesh()
{
  ID3D11Buffer* vertexBuffer;
  D3D11_BUFFER_DESC vertexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData;

  ID3D11Buffer* indexBuffer;
  D3D11_BUFFER_DESC indexBufferDesc;
  D3D11_SUBRESOURCE_DATA indexData;

  HRESULT result;


  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertices.size();
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  vertexData.pSysMem = vertices.data();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  // Now create the vertex buffer.
  result = m_Device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

  // Set up the description of the static index buffer.
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * indices.size();
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the index data.
  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = m_Device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

  Mesh* mesh = new Mesh(vertexBuffer, indexBuffer, vertices.size(), indices.size());

  // Clear vectors now that the vertex and index buffers have been created and loaded.
  vertices.clear();
  indices.clear();


  return mesh;
}

// -----------------------------------------------------------------------------

void MeshGenerator::makePlane(int subW, int subH, PlaneType type)
{
  //float dW = (1.0f / (float)subW);
  //float dH = (1.0f / (float)subH);


  //XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
  //XMFLOAT2 UV = XMFLOAT2(0.0f, 0.0f);

  //int row = (subW + 1);
  //int column = (subH + 1);

  //XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, -1.0f);  // Default


  //for (int j = 0; j < column; j++)
  //{
  //  for (int i = 0; i < row; i++)
  //  {
  //    position.x = LEFT_OFFSET + (i * dW);
  //    position.y = TOP_OFFSET - (j * dH);
  //    UV.x = (i * dW);
  //    UV.y = (j * dH);

  //    // Load the vertex array with data.
  //    vertices.push_back({ position, UV, normal });
  //  }    
  //}


  //for (int j = 0; j < subH; j++)
  //{
  //  for (int i = 0; i < subW; i++)
  //  {
  //    // Load the index array with data.
  //    indices.push_back( (j*row) + i );             // Top left
  //    indices.push_back( ((j + 1)*row) + i );       // Bottom left
  //    indices.push_back( ((j + 1)*row) + (i + 1) ); // Bottom right
  //    indices.push_back( ((j + 1)*row) + (i + 1) ); // Bottom right
  //    indices.push_back( (j*row) + (i + 1));        // Top right
  //    indices.push_back( (j*row) + i );             // Top left
  //  }
  //}

}

// -----------------------------------------------------------------------------











// 80 //////////////////////////////////////////////////////////////////////////