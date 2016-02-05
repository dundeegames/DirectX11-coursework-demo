////////////////////////////////////////////////////////////////////////////////
/**
* \file       Mesh.cpp
* \brief      Mesh class -
*
* \details    Detailed description
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
#include"Mesh.h"
#include<System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


Mesh::Mesh(ID3D11Buffer* vBuffer, ID3D11Buffer* iBuffer,
            int vCount, int iCount) : m_vertexBuffer(vBuffer), m_indexBuffer(iBuffer),
                                      m_vertexCount(vCount), m_indexCount(iCount),
                                      m_Texture(nullptr)
{
  ZeroMemory(&position, sizeof(XMFLOAT3));
  ZeroMemory(&rotation, sizeof(XMFLOAT3));
  scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}


Mesh::~Mesh()
{
  RELEASE_OBJECT(m_indexBuffer);
  RELEASE_OBJECT(m_vertexBuffer);
}

// -----------------------------------------------------------------------------

void Mesh::SendData(ID3D11DeviceContext* deviceContext)
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

  // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

// -----------------------------------------------------------------------------

ID3D11ShaderResourceView* Mesh::GetTexture()
{
  if (m_Texture != nullptr)
  {
    return m_Texture->GetTexture();
  }
  else
  {
    return nullptr;
  }
}

// -----------------------------------------------------------------------------

void Mesh::setPosition(float x, float y, float z)
{
  position = XMFLOAT3(x, y, z);
}

// -----------------------------------------------------------------------------

void Mesh::setRotation(float x, float y, float z)
{
  rotation = XMFLOAT3(x, y, z);
}

// -----------------------------------------------------------------------------

void Mesh::setScale(float x, float y, float z)
{
  scale = XMFLOAT3(x, y, z);
}

// -----------------------------------------------------------------------------

void Mesh::move(float x, float y, float z)
{
  position.x += x;
  position.y += y;
  position.z += z;
}

// -----------------------------------------------------------------------------

void Mesh::rotate(float x, float y, float z)
{
  rotation.x += x;
  rotation.y += y;
  rotation.z += z;
}

// -----------------------------------------------------------------------------

XMMATRIX Mesh::getWorldMatrix()
{
  XMMATRIX world;

  world = XMMatrixScaling(scale.x, scale.y, scale.z)
            * XMMatrixRotationRollPitchYaw( RADIANS(rotation.x),
                                            RADIANS(rotation.y),
                                            RADIANS(rotation.z))
            * XMMatrixTranslation(position.x, position.y, position.z);
  
  return world;
}









// 80 //////////////////////////////////////////////////////////////////////////