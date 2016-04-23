////////////////////////////////////////////////////////////////////////////////
/**
* \file       Simplex2DheightShader.h
* \brief      Simplex2DheightShader class - 
*
* \details    Detailed description 
*
* \author     Jiri Klic
* \version    1.0
* \date       April 2016
* \pre        
* \post       
* \bug        No known bugs
* \warning    
* \todo       
*             
* \copyright  University of Abertay - Dundee.2016.
*             Intellectual Property Policy.[online].Available from:
*             http://www.abertay.ac.uk/media/intellectual%20property%20policy%20january%202016.pdf
*             [Accessed 19 March 2016].
*
*///////////////////////////////////////////////////////////////////////////////


#ifndef SIMPLEX2D_HEIGHT_SHADER_H
#define SIMPLEX2D_HEIGHT_SHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"

using namespace std;
using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

class Simplex2DheightShader : public BaseShader
{

  struct Simplex2DBufferType
  {
    float frequency;
    float amplitude;
    float xOffset;
    float yOffset;
  };

public:
  Simplex2DheightShader(ID3D11Device* device, HWND hwnd);
  ~Simplex2DheightShader();

  void InitShader(WCHAR* vsFilename, WCHAR* psFilename);

  void SetShaderParameters(ID3D11DeviceContext* deviceContext,
                           const XMMATRIX &worldMatrix,
                           const XMMATRIX &viewMatrix,
                           const XMMATRIX &projectionMatrix,
                           float freq = 1.0f, float ampl = 1.0f,
                           float xOff = 0.0f, float yOff = 0.0f);


  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);


private:
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_Simplex2DBuffer;

};


#endif // SIMPLEX2D_HEIGHT_SHADER_H




// 80 //////////////////////////////////////////////////////////////////////////