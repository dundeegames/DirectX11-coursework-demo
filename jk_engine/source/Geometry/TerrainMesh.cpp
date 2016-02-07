////////////////////////////////////////////////////////////////////////////////
/**
* \file       TerrainMesh.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "TerrainMesh.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


TerrainMesh::TerrainMesh(ID3D11Device* device, WCHAR* textureFilename,
                         float width, float height, int subW, int subH)
{
  // Generate PAtch control points
  GeneratePatch(width, height, subW, subH);

  // Initialize the buffers.
  InitBuffers(device);

  // Load the texture for this model.
  //LoadTexture(device, textureFilename);
  // Create the texture object.
}


TerrainMesh::~TerrainMesh()
{
  // Run parent deconstructor
  BaseMesh::~BaseMesh();
}

// -----------------------------------------------------------------------------

void TerrainMesh::GeneratePatch(float width, float height, int subW, int subH)
{
  const float LEFT_OFFSET = width * (-0.5f);
  const float TOP_OFFSET = height * 0.5f;

  float dU = (1.0f / (float)subW);
  float dV = (1.0f / (float)subH);
  float dW = width * dU;
  float dH = height * dV;
  int row = (subW + 1);
  int column = (subH + 1);

  XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
  XMFLOAT2 UV = XMFLOAT2(0.0f, 0.0f);
  XMFLOAT3 normal = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Default

  for (int j = 0; j < column; j++)
  {
    for (int i = 0; i < row; i++)
    {
      position.x = LEFT_OFFSET + (i * dW);
      position.z = TOP_OFFSET - (j * dH);
      UV.x = (i * dU);
      UV.y = (j * dV);

      // Load the vertex array with data.
      m_Vertices.push_back({ position, UV, normal });
    }
  }


  for (int j = 0; j < subH; j++)
  {
    for (int i = 0; i < subW; i++)
    {
      // Load the index array with data.
      m_Indices.push_back((j*row) + i);             // Top left
      m_Indices.push_back(((j + 1)*row) + i);       // Bottom left
      m_Indices.push_back((j*row) + (i + 1));       // Top right
      m_Indices.push_back(((j + 1)*row) + (i + 1)); // Bottom right
    }
  }

}

// -----------------------------------------------------------------------------

void TerrainMesh::InitBuffers(ID3D11Device* device)
{
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;

  // Set the number of vertices in the vertex array.
  m_vertexCount = m_Vertices.size();

  // Set the number of indices in the index array.
  m_indexCount = m_Indices.size();



  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  vertexData.pSysMem = m_Vertices.data();
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
  indexData.pSysMem = m_Indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

  // Release the arrays now that the vertex and index buffers have been created and loaded.
  m_Vertices.clear();
  m_Indices.clear();

}

// -----------------------------------------------------------------------------

void TerrainMesh::SendData(ID3D11DeviceContext* deviceContext)
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