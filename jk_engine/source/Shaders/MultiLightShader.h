////////////////////////////////////////////////////////////////////////////////
/**
* \file       MultiLightShader.h
* \brief      MultiLightShader class -
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


#ifndef MULTILIGHTSHADER_H
#define MULTILIGHTSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include "BaseShader.h"
#include <System/Light.h>


using namespace std;
using namespace DirectX;


#define MAX_LIGHTS 4

// CLASS ///////////////////////////////////////////////////////////////////////

class MultiLightShader : public BaseShader
{
private:
  struct PointBufferType
  {
    XMFLOAT4 diffuse[MAX_LIGHTS];
    //XMFLOAT4 ambient[MAX_LIGHTS];
    XMFLOAT4 position[MAX_LIGHTS];
    //bool isLife[MAX_LIGHTS];
  };


public:

  MultiLightShader(ID3D11Device* device, HWND hwnd);
  ~MultiLightShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
    const XMMATRIX &view, const XMMATRIX &projection,
    ID3D11ShaderResourceView* texture, std::vector<Light*>& lights);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);
  void InitShader(WCHAR*, WCHAR*);

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11SamplerState* m_sampleState;

};


#endif // MULTILIGHTSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////