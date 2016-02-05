////////////////////////////////////////////////////////////////////////////////
/**
* \file       TessellationMesh.cpp
* \brief      TessellationMesh class -
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
#include "tessellationmesh.h"
#include <vector>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


TessellationMesh::TessellationMesh(ID3D11Device* device, WCHAR* textureFilename)
{
  // Initialize the vertex and index buffer that hold the geometry for the triangle.
  InitBuffers(device);

  // Load the texture for this model.
  LoadTexture(device, textureFilename);
}


TessellationMesh::~TessellationMesh()
{
  // Run parent deconstructor
  BaseMesh::~BaseMesh();
}

// -----------------------------------------------------------------------------

void TessellationMesh::InitBuffers(ID3D11Device* device)
{
  VertexType tempVertex;  // Temporary Vertex
  std::vector<VertexType> vertices;
  std::vector<unsigned long> indices;

  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;


  tempVertex.position = XMFLOAT3(-25.0f, 0.0f, 25.0f);  // Top Left
  tempVertex.texture = XMFLOAT2(0.0f, 0.0f);
  tempVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
  vertices.push_back(tempVertex);

  tempVertex.position = XMFLOAT3(25.0f, 0.0f, 25.0f);  // Top right
  tempVertex.texture = XMFLOAT2(1.0f, 0.0f);
  tempVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
  vertices.push_back(tempVertex);

  tempVertex.position = XMFLOAT3(-25.0f, 0.0f, -25.0f);  // Bottom left.
  tempVertex.texture = XMFLOAT2(0.0f, 1.0f);
  tempVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
  vertices.push_back(tempVertex);

  //tempVertex.position = XMFLOAT3(25.0f, 0.0f, 25.0f);  // Top right
  //tempVertex.texture = XMFLOAT2(1.0f, 0.0f);
  //tempVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
  //vertices.push_back(tempVertex);

  tempVertex.position = XMFLOAT3(25.0f, 0.0f, -25.0f);  // Bottom right.
  tempVertex.texture = XMFLOAT2(1.0f, 1.0f);
  tempVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
  vertices.push_back(tempVertex);


  //Load the index vector with data.
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(1);
  //indices.push_back(2);
  indices.push_back(3);


  // Set the number of vertices in the vertex array.
  m_vertexCount = vertices.size();

  // Set the number of indices in the index array.
  m_indexCount = indices.size();



  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  vertexData.pSysMem = vertices.data();
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
  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

  // Release the arrays now that the vertex and index buffers have been created and loaded.
  vertices.clear();
  indices.clear();

}

// -----------------------------------------------------------------------------

void TessellationMesh::SendData(ID3D11DeviceContext* deviceContext)
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
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////