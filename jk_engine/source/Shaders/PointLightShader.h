////////////////////////////////////////////////////////////////////////////////
/**
* \file       PointLightShader.h
* \brief      PointLightShader class - 
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


#ifndef POINTLIGHTSHADER_H
#define POINTLIGHTSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Light.h>
#include <System/Camera.h>

using namespace std;
using namespace DirectX;

// CLASS ///////////////////////////////////////////////////////////////////////

class PointLightShader : public BaseShader
{
private:
  struct PointBufferType
  {
    XMFLOAT4 diffuse;
    XMFLOAT4 ambient;
    XMFLOAT3 position;
    float padding;
  };

  struct CameraBufferType
  {
    XMFLOAT3 position;
    float padding;
  };

public:

  PointLightShader(ID3D11Device* device, HWND hwnd);
  ~PointLightShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
                            const XMMATRIX &view, const XMMATRIX &projection,
                            ID3D11ShaderResourceView* texture, Light* light, Camera* camera);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);
  void InitShader(WCHAR*, WCHAR*);  

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11Buffer* m_cameraBuffer;
  ID3D11SamplerState* m_sampleState;

};


#endif // POINTLIGHTSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////