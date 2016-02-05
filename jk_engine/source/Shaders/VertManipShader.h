////////////////////////////////////////////////////////////////////////////////
/**
* \file       VertManipShader.h
* \brief      VertManipShader class -
*
* \details    Detailed description
*
* \author     Jiri Klic
* \version    1.0
* \date       October 2015
* \pre        call InitShader() before use
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


#ifndef VERTMANIPSHADER_H
#define VERTMANIPSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Light.h>
#include <System/Camera.h>
#include <System/Timer.h>

using namespace std;
using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////


class VertManipShader : public BaseShader
{
private:
  struct SpecularBufferType
  {
    XMFLOAT4 diffuse;
    XMFLOAT4 ambient;
    XMFLOAT3 direction;
    float specularPower;
    XMFLOAT4 specular;
  };

  struct CameraBufferType
  {
    XMFLOAT3 position;
    float time;         // Passing for vert manipulation
  };


public:

  VertManipShader(ID3D11Device* device, HWND hwnd);
  ~VertManipShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
    const XMMATRIX &view, const XMMATRIX &projection,
    ID3D11ShaderResourceView* texture, Light* light, Camera* camera, Timer* timer);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);
  void InitShader(WCHAR*, WCHAR*);

private:


private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11Buffer* m_cameraBuffer;
  ID3D11SamplerState* m_sampleState;
  float time;
  float speed;
};


#endif // VERTMANIPSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////