////////////////////////////////////////////////////////////////////////////////
/**
* \file       PointMesh.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "pointmesh.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


PointMesh::PointMesh(ID3D11Device* device, WCHAR* textureFilename)
{
  // Initialize the vertex and index buffer that hold the geometry for the triangle.
  InitBuffers(device);

  // Load the texture for this model.
  LoadTexture(device, textureFilename);
}


PointMesh::~PointMesh()
{
  // Run parent deconstructor
  BaseMesh::~BaseMesh();
}


// -----------------------------------------------------------------------------

void PointMesh::InitBuffers(ID3D11Device* device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;

  // Set the number of vertices in the vertex array.
  m_vertexCount = 3;

  // Set the number of indices in the index array.
  m_indexCount = 3;

  // Create the vertex array.
  vertices = new VertexType[m_vertexCount];

  // Create the index array.
  indices = new unsigned long[m_indexCount];

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(0.0f, 10.0f, 0.0f);  // Top.
  vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
  vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  vertices[1].position = XMFLOAT3(-10.0f, 0.0f, 0.0f);  // Bottom left.
  vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
  vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  vertices[2].position = XMFLOAT3(10.0f, 0.0f, 0.0f);  // Bottom right.
  vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
  vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

  // Load the index array with data.
  indices[0] = 0;  // Top/
  indices[1] = 1;  // Bottom left.
  indices[2] = 2;  // Bottom right.

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
  device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

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
  device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

  // Release the arrays now that the vertex and index buffers have been created and loaded.
  delete[] vertices;
  vertices = 0;

  delete[] indices;
  indices = 0;
}

// -----------------------------------------------------------------------------

void PointMesh::SendData(ID3D11DeviceContext* deviceContext)
{
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(VertexType);
  offset = 0;

  // Set the vertex buffer to active in the input assembler so it can be rendered.
  deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  // Set the index buffer to active in the input assembler so it can be rendered.
  deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  // Set the type of primitive that should be rendered from this vertex buffer, in this case control patch for tessellation.
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

// -----------------------------------------------------------------------------









// 80 //////////////////////////////////////////////////////////////////////////