////////////////////////////////////////////////////////////////////////////////
/**
* \file       QuadMesh.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "QuadMesh.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


QuadMesh::QuadMesh(ID3D11Device* device, WCHAR* textureFilename)
{
  device_ = device;
  // Initialize the vertex and index buffer that hold the geometry for the triangle.
  InitBuffers(device_);

  // Load the texture for this model.
  LoadTexture(device_, textureFilename);
}


QuadMesh::~QuadMesh()
{
  // Run parent deconstructor
  BaseMesh::~BaseMesh();
}


// -----------------------------------------------------------------------------

void QuadMesh::setTexture(WCHAR* textureFilename)
{
  LoadTexture(device_, textureFilename);
}

// -----------------------------------------------------------------------------

void QuadMesh::InitBuffers(ID3D11Device* device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  // Set the number of vertices in the vertex array.
  m_vertexCount = 4;

  // Set the number of indices in the index array.
  m_indexCount = 6;

  // Create the vertex array.
  vertices = new VertexType[m_vertexCount];

  // Create the index array.
  indices = new unsigned long[m_indexCount];

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
  vertices[0].texture = XMFLOAT2(0.0f, 0.0f);
  vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  vertices[1].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
  vertices[1].texture = XMFLOAT2(0.0f, 1.0f);
  vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
  vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
  vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
  vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
  vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);



  // Load the index array with data.
  indices[0] = 0;  // Top left
  indices[1] = 1;  // Bottom left.
  indices[2] = 2;  // Bottom right.
  indices[3] = 2;  // Bottom right.
  indices[4] = 3;  // Top right.
  indices[5] = 0;  // Top left.

  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  // Now create the vertex buffer.
  result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

  // Set up the description of the static index buffer.
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the index data.
  indexData.pSysMem = indices;
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

  // Release the arrays now that the vertex and index buffers have been created and loaded.
  delete[] vertices;
  vertices = 0;

  delete[] indices;
  indices = 0;
}

// -----------------------------------------------------------------------------







// 80 //////////////////////////////////////////////////////////////////////////