////////////////////////////////////////////////////////////////////////////////
/**
* \file       UberTessellShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "UberTessellShader.h"
#include  <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


UberTessellShader::UberTessellShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd),
                              m_matrixBuffer(nullptr), m_lightBuffer(nullptr),
                              m_cameraBuffer(nullptr), m_sampleState(nullptr),
                              m_Texture(nullptr), m_Heightmap(nullptr), m_Camera(nullptr),
                              m_Hwnd(hwnd), m_TessellationFactor(0.0f)
{

}


UberTessellShader::~UberTessellShader()
{
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_textureMatrixBuffer);
  RELEASE_OBJECT(m_layout);
  RELEASE_OBJECT(m_lightBuffer);
  RELEASE_OBJECT(m_cameraBuffer);

  //Release base shader components
  BaseShader::~BaseShader();
}


// -----------------------------------------------------------------------------

void UberTessellShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_BUFFER_DESC textureMatrixBufferDesc;
  D3D11_BUFFER_DESC lightBufferDesc;
  D3D11_BUFFER_DESC cameraBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;

  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  ZeroMemory(&textureMatrixBufferDesc, sizeof(textureMatrixBufferDesc));
  ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
  ZeroMemory(&cameraBufferDesc, sizeof(cameraBufferDesc));
  ZeroMemory(&samplerDesc, sizeof(samplerDesc));

  // Load (+ compile) shader files
  loadVertexShader(vsFilename);
  loadPixelShader(psFilename);

  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);


  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  textureMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  textureMatrixBufferDesc.ByteWidth = sizeof(TextureMatrixBufferType);
  textureMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  textureMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  textureMatrixBufferDesc.MiscFlags = 0;
  textureMatrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_textureMatrixBuffer);


  // Create a texture sampler state description.
  // TODO: change mirroring back to tiling
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // Create the texture sampler state.
  m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

  /**
  * Setup light buffer
  * Setup the description of the light dynamic constant buffer that is in the pixel shader.
  * Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER
  * or CreateBuffer will fail.
  */
  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBufferType);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
  m_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);


  // Setup the description of the dynamic camera constant buffer that is in the vertex shader.
  cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
  cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cameraBufferDesc.MiscFlags = 0;
  cameraBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
  m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);

}

// -----------------------------------------------------------------------------

void UberTessellShader::InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
  // InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
  InitShader(vsFilename, psFilename);

  // Load other required shaders.
  loadHullShader(hsFilename);
  loadDomainShader(dsFilename);
}

// -----------------------------------------------------------------------------

void UberTessellShader::setLights(std::vector<Light*>& lights)
{
#ifdef _DEBUG
  if (lights.size() > MAX_LIGHTS)
  {
    Common::die("You are adding too many Lights in your vector!\n");
  }
#endif

  if (!m_Lights.empty())
  {
    m_Lights.clear();
  }

  int numLights = max(lights.size(), MAX_LIGHTS);
  m_Lights.assign(lights.begin(), (lights.begin() + numLights));

}

// -----------------------------------------------------------------------------

void UberTessellShader::addLight(Light* light)
{
#ifdef _DEBUG
  if (m_Lights.size() == MAX_LIGHTS)
  {
    Common::die("You cannot add another Light to UberShader!\n");
  }
#endif

  if (m_Lights.size() < MAX_LIGHTS)
  {
    m_Lights.push_back(light);
  }

}

// -----------------------------------------------------------------------------

void UberTessellShader::setView(ID3D11DeviceContext* deviceContext,
                                const XMMATRIX &worldMatrix,
                                const XMMATRIX &viewMatrix,
                                const XMMATRIX &projectionMatrix,
                                const XMMATRIX &textureMatrix)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  TextureMatrixBufferType* textureMatrixPtr;

  CameraBufferType* camPtr;
  LightBufferType* lightPtr;
  LightBufferType zeroLightBuffer;
  //TessellationBufferType* tessellationPtr;

  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj, texMatrix;
  XMFLOAT3 tempPosition;

  ZeroMemory(&mappedResource, sizeof(mappedResource));
  ZeroMemory(&zeroLightBuffer, sizeof(LightBufferType));

  // Transpose the matrices to prepare them for the shader.
  texMatrix = XMMatrixTranspose(textureMatrix);

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_textureMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  textureMatrixPtr = (TextureMatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  textureMatrixPtr->TextureMatrix = texMatrix;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_textureMatrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_textureMatrixBuffer);

  //---

  // Transpose the matrices to prepare them for the shader.
  tworld = XMMatrixTranspose(worldMatrix);
  tview = XMMatrixTranspose(viewMatrix);
  tproj = XMMatrixTranspose(projectionMatrix);

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  dataPtr->world = tworld;// worldMatrix;
  dataPtr->view = tview;
  dataPtr->projection = tproj;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;
  

  // if Hull shader is not null then set DS, else set VS
  if (m_hullShader)
  {
    // Now set the constant buffer in the domain shader with the updated values.
    deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
  }
  else
  {
    // Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
  }


  // CAMERA: ---------
  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  camPtr = (CameraBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  camPtr->cameraPosition = m_Camera->GetPosition();// worldMatrix;
  camPtr->tessellationFactor = m_TessellationFactor;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_cameraBuffer, 0);


  // if Hull shader is not null then set DS, else set VS
  if (m_hullShader)
  {
    // Set the position of the constant buffer in the vertex shader.
    bufferNumber = 0;

    // Now set the constant buffer in the hull shader with the updated values.
    deviceContext->HSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
  }
  else
  {
    // Set the position of the constant buffer in the vertex shader.
    bufferNumber = 1;

    // Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
  }




  // LIGHTS: ---------
  if (m_Lights.size() > 0)
  {
    // Lock the light constant buffer so it can be written to
    deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    // Get a pointer to the data in the constant buffer
    lightPtr = (LightBufferType*)mappedResource.pData;

    //ZeroMemory
    *lightPtr = zeroLightBuffer;

    for (unsigned int i = 0; i < m_Lights.size(); i++)
    {
      lightPtr->lightData[i].ambient = m_Lights[i]->GetAmbientColour();
      lightPtr->lightData[i].diffuse = m_Lights[i]->GetDiffuseColour();
      tempPosition = m_Lights[i]->GetPosition();
      lightPtr->lightData[i].position = XMFLOAT4(tempPosition.x, tempPosition.y, tempPosition.z, 1.0f);
      lightPtr->lightData[i].specular = m_Lights[i]->GetSpecularColour();
      lightPtr->lightData[i].specularPower = m_Lights[i]->GetSpecularPower();
      lightPtr->lightData[i].isActive = SWITCH_ON;
      lightPtr->lightData[i].hasSpecularity = SWITCH_ON;
    }

    // Unlock the buffer
    deviceContext->Unmap(m_lightBuffer, 0);
    // Set buffer number (register value)
    bufferNumber = 0;
    // Set the constant buffer in the pixel shader
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

  }

  //---------

  // Set shader texture resource in the domain shader.
  deviceContext->DSSetShaderResources(0, 1, &m_Heightmap);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &m_Texture);
}

// -----------------------------------------------------------------------------

void UberTessellShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // if Hull shader is not null then set the sampler state in the domain shader.
  if (m_hullShader)
  {
    deviceContext->DSSetSamplers(0, 1, &m_sampleState);
  }

  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}

// -----------------------------------------------------------------------------




// 80 //////////////////////////////////////////////////////////////////////////