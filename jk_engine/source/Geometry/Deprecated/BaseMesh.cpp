////////////////////////////////////////////////////////////////////////////////
/**
* \file       BaseMesh.cpp
* \brief      BaseMesh class -
*
* \details    Base mesh class, for inheriting base mesh functionality
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
#include "basemesh.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


BaseMesh::BaseMesh()
{
  m_Texture = nullptr;
}


BaseMesh::~BaseMesh()
{
  // Release the index buffer.
  if (m_indexBuffer)
  {
    m_indexBuffer->Release();
    m_indexBuffer = 0;
  }

  // Release the vertex buffer.
  if (m_vertexBuffer)
  {
    m_vertexBuffer->Release();
    m_vertexBuffer = 0;
  }

  // Release the texture object.
  if (m_Texture)
  {
    delete m_Texture;
    m_Texture = 0;
  }

}


// -----------------------------------------------------------------------------

int BaseMesh::GetIndexCount()
{
  return m_indexCount;
}

// -----------------------------------------------------------------------------

ID3D11ShaderResourceView* BaseMesh::GetTexture()
{
  return m_Texture->GetTexture();
}

// -----------------------------------------------------------------------------

void BaseMesh::SendData(ID3D11DeviceContext* deviceContext)
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

void BaseMesh::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
  // Release the previous texture object.
  if (m_Texture)
  {
    delete m_Texture;
    m_Texture = nullptr;
  }

  // Create the texture object.
  m_Texture = new Texture(device, filename);

}

// -----------------------------------------------------------------------------







// 80 //////////////////////////////////////////////////////////////////////////











