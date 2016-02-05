////////////////////////////////////////////////////////////////////////////////
/**
* \file       TextureShader.h
* \brief      TextureShader class -
*
* \details    
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


#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"

using namespace std;
using namespace DirectX;


// DEFINES /////////////////////////////////////////////////////////////////////
#define NORMAL        0
#define GRAYSCALE     1
#define INVERSE       2
#define COLOURED      3
#define STATE_ERROR   -1


// CLASS ///////////////////////////////////////////////////////////////////////

class TextureShader : public BaseShader
{
  struct StateBufferType
  {
    XMFLOAT3 colour;
    int state;
  };

  
public:

  TextureShader(ID3D11Device* device, HWND hwnd);
  ~TextureShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext,
                            const XMMATRIX &world, const XMMATRIX &view,
                            const XMMATRIX &projection, ID3D11ShaderResourceView* texture,
                            int state = NORMAL, XMFLOAT3 colour = XMFLOAT3(1.0f, 1.0f, 1.0f));

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

  void InitShader(WCHAR*, WCHAR*);

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_stateBuffer;
  ID3D11SamplerState* m_sampleState;
};


#endif // TEXTURESHADER_H




// 80 //////////////////////////////////////////////////////////////////////////