////////////////////////////////////////////////////////////////////////////////
/**
* \file       ShadowShader.h
* \brief      ShadowShader class - 
*
* \details    Detailed description 
*
* \author     Paul Robertson
* \version    1.0
* \date       November 2015
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


#ifndef SHADOWSHADER_H
#define SHADOWSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Light.h>

using namespace std;
using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

class ShadowShader : public BaseShader
{
private:
  struct MatrixBufferType
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
    XMMATRIX lightView;
    XMMATRIX lightProjection;
  };

  struct LightBufferType
  {
    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
  };

  struct LightBufferType2
  {
    XMFLOAT3 position;
    float padding;
  };

public:

  ShadowShader(ID3D11Device* device, HWND hwnd);
  ~ShadowShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world,
                           const XMMATRIX& view, const XMMATRIX& projection,
                           ID3D11ShaderResourceView* texture,
                           ID3D11ShaderResourceView* depthMap, Light* light);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

  void InitShader(WCHAR* vsFilename, WCHAR* psFilename);

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11SamplerState* m_sampleState;
  ID3D11SamplerState* m_sampleStateClamp;
  ID3D11Buffer* m_LightBuffer;
  ID3D11Buffer* m_LightBuffer2;

};


#endif // SHADOWSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////