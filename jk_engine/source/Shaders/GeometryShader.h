////////////////////////////////////////////////////////////////////////////////
/**
* \file       GeometryShader.h
* \brief      GeometryShader class -
*
* \details    Geometry shader example.
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


#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Camera.h>

using namespace std;
using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

class GeometryShader : public BaseShader
{
  struct CameraBufferType
  {
    XMFLOAT3 position;
    float padding1;
    XMFLOAT4 up;
  };



public:

  GeometryShader(ID3D11Device* device, HWND hwnd);
  ~GeometryShader();

  void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
                           const XMMATRIX &view, const XMMATRIX &projection,
                           ID3D11ShaderResourceView* texture, Camera* camera);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

  void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
  void InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

  void enableShader(ID3D11DeviceContext* deviceContext);
  void dissableShader(ID3D11DeviceContext* deviceContext);

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_cameraBuffer;
  ID3D11SamplerState* m_sampleState;
  
};


#endif // GEOMETRYSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////