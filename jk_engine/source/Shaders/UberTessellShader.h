////////////////////////////////////////////////////////////////////////////////
/**
* \file       UberTessellShader.h
* \brief      UberTessellShader class -
*
* \details    Shader handling multiple lights, spec ularity, textures
*             and tesselation
*
* \author     Jiri Klic
* \version    1.0
* \date       February 2016
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


#ifndef UBERTESSELLSHADER_H
#define UBERSHADER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "BaseShader.h"

#include <System/Light.h>
#include <System/Camera.h>
#include <vector>

#define MAX_LIGHTS 8
#define SWITCH_ON 1
#define SWITCH_OFF 0

// CLASS ///////////////////////////////////////////////////////////////////////

class UberTessellShader : public BaseShader
{
private:

  struct LightData
  {
    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
    XMFLOAT4 position;  // =1 directional light, <1 point light
    XMFLOAT4 specular;
    float specularPower;
    int isActive;
    int hasSpecularity;
    int padding;
  };

  struct LightBufferType
  {
    LightData lightData[MAX_LIGHTS];
  };

  struct CameraBufferType
  {
    XMFLOAT3 position;
    float tessellationFactor;       // if tessellation off, used as a padding
  };

public:
  UberTessellShader(ID3D11Device* device, HWND hwnd);
  ~UberTessellShader();

  void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
  void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

  void setTexture(ID3D11ShaderResourceView* texture) { m_Texture = texture; }
  void setCamera(Camera* camera) { m_Camera = camera; }

  /**
  * Accepts only MAX_LIGHTS number of lights!
  * Overwrites the previous lights
  */
  void setLights(std::vector<Light*>& lights);

  ///< Accepts only MAX_LIGHTS number of lights!
  void addLight(Light* light);

  void setView(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
    const XMMATRIX &view, const XMMATRIX &projection);

  void Render(ID3D11DeviceContext* deviceContext, int vertexCount);




private:
  HWND m_Hwnd;
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11Buffer* m_cameraBuffer;
  ID3D11Buffer* m_tessellationBuffer;
  ID3D11SamplerState* m_sampleState;

  /**
  * memory allocations of these pointers are released elsewhere
  */
  ID3D11ShaderResourceView* m_Texture;
  Camera* m_Camera;
  std::vector<Light*> m_Lights;

};


#endif // UBERSHADER_H




// 80 //////////////////////////////////////////////////////////////////////////