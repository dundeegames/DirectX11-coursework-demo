////////////////////////////////////////////////////////////////////////////////
/**
* \file       BoxBlurShader.h
* \brief      BoxBlurShader class - 
*
* \details    Detailed description 
*
* \author     Jiri Klic
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


#ifndef BOXBLURSHADER_H
#define BOXBLURSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"
#include <System/Light.h>


using namespace std;
using namespace DirectX;

// CLASS ///////////////////////////////////////////////////////////////////////

struct ScreenSizeBufferType
{
  float screenWidth;
  float screenHeight;
  XMFLOAT2 padding;

  ScreenSizeBufferType()
  {
    screenWidth = 0.0f;
    screenHeight = 0.0f;
    padding = XMFLOAT2(0.0f, 0.0f);
  }

};


class BoxBlurShader : public BaseShader
{

public:
  BoxBlurShader(ID3D11Device* device, HWND hwnd);
  ~BoxBlurShader();

  void InitShader(WCHAR*, WCHAR*);

  void SetShaderParameters(ID3D11DeviceContext* deviceContext,
            const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
            ID3D11ShaderResourceView* texture, ScreenSizeBufferType* scrBuff );

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11SamplerState* m_sampleState;
  ID3D11Buffer* m_ScreenSizeBuffer;

};


#endif // BOXBLURSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////