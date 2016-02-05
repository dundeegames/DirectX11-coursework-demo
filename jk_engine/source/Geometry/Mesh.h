////////////////////////////////////////////////////////////////////////////////
/**
* \file       Mesh.h
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


#ifndef MESH_H
#define MESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <d3d11.h>
#include <directxmath.h>
#include "texture.h"

using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

struct VertexType
{
  XMFLOAT3 position;
  XMFLOAT2 texture;
  XMFLOAT3 normal;

};


class Mesh
{

public:
  Mesh(ID3D11Buffer* vBuffer, ID3D11Buffer* iBuffer, int vCount, int iCount);
  ~Mesh();

  void init(ID3D11Buffer* vBuffer, ID3D11Buffer* iBuffer,
              int vCount, int iCount, const Texture* texture);

  void SendData(ID3D11DeviceContext* deviceContext);
  int GetIndexCount() const { return m_indexCount; }
  ID3D11ShaderResourceView* GetTexture();

  void setTexture(Texture* texture) { m_Texture = texture; }
  void setPosition(float x, float y, float z);
  void setRotation(float x, float y, float z);
  void setScale(float x, float y, float z);

  void move(float x, float y, float z);
  void rotate(float x, float y, float z);

  XMMATRIX getWorldMatrix();




private:
  XMFLOAT3 position;
  XMFLOAT3 rotation;
  XMFLOAT3 scale;

  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
  int m_vertexCount;
  int m_indexCount;
  Texture* m_Texture;

};


#endif // MESH_H




// 80 //////////////////////////////////////////////////////////////////////////