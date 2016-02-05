////////////////////////////////////////////////////////////////////////////////
/**
* \file       TessellationShader.h
* \brief      TessellationShader class - 
*
* \details     
*
* \author     Jiri Klic
* \version    2.0
* \date       December 2015
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


#ifndef TESSELLATIONSHADER_H
#define TESSELLATIONSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Camera.h>

using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

class TessellationShader : public BaseShader
{
private:

  struct TessellationBufferType
  {
    float tessellationFactor;
    XMFLOAT3 cameraPosition;
  };

  struct  TextureMatrixBufferType
  {
    XMMATRIX TextureMatrix;
  };


public:

  TessellationShader(ID3D11Device* device, HWND hwnd);
  ~TessellationShader();

  void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
  void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

  void SetShaderParameters(ID3D11DeviceContext* deviceContext,
                            const XMMATRIX &world, const XMMATRIX &view,
                            const XMMATRIX &projection, const XMMATRIX &textureMatrix,
                            ID3D11ShaderResourceView* heightmap, Camera* camera,
                            float tessFactor = 1.0f);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);


private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_textureMatrixBuffer;
  ID3D11SamplerState* m_sampleState;
  ID3D11SamplerState* m_domainSampleState;
  ID3D11Buffer* m_tessellationBuffer;
};


#endif // TESSELLATIONSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////