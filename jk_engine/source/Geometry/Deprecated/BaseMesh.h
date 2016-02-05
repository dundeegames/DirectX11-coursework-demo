////////////////////////////////////////////////////////////////////////////////
/**
* \file       BaseMesh.h
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


#ifndef BASEMESH_H
#define BASEMESH_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <d3d11.h>
#include <directxmath.h>
#include "../texture.h"

using namespace DirectX;





// CLASS ///////////////////////////////////////////////////////////////////////

class BaseMesh
{
protected:

  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT2 texture;
    XMFLOAT3 normal;
  };

public:
  BaseMesh();
  ~BaseMesh();

  virtual void SendData(ID3D11DeviceContext*);
  //void SendData(ID3D11DeviceContext*);
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();

protected:
  virtual void InitBuffers(ID3D11Device*) = 0;
  void LoadTexture(ID3D11Device*, WCHAR*);

  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;
  Texture* m_Texture;

};


#endif // BASEMESH_H




// 80 //////////////////////////////////////////////////////////////////////////