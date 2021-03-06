////////////////////////////////////////////////////////////////////////////////
/**
* \file       ShadowShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "shadowshader.h"
#include <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
  
}


ShadowShader::~ShadowShader()
{
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_layout);
  RELEASE_OBJECT(m_LightBuffer);
  RELEASE_OBJECT(m_LightBuffer2);

  //Release base shader components
  BaseShader::~BaseShader();
}


// -----------------------------------------------------------------------------

void ShadowShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC lightBufferDesc;
  D3D11_BUFFER_DESC lightBufferDesc2;

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

  // Create a texture sampler state description.
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
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

  // Required a CLAMPED sampler for sampling the depth map
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  // Create the texture sampler state.
  m_device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);


  // Setup light buffer
  // Setup the description of the light dynamic constant buffer that is in the pixel shader.
  // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBufferType);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&lightBufferDesc, NULL, &m_LightBuffer);

  // Setup the description of the camera dynamic constant buffer that is in the vertex shader.
  lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
  lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc2.MiscFlags = 0;
  lightBufferDesc2.StructureByteStride = 0;

  // Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&lightBufferDesc2, NULL, &m_LightBuffer2);

}


void ShadowShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, Light* light)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  LightBufferType* lightPtr;
  LightBufferType2* lightPtr2;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj, tLightViewMatrix, tLightProjectionMatrix;


  // Transpose the matrices to prepare them for the shader.
  tworld = XMMatrixTranspose(worldMatrix);
  tview = XMMatrixTranspose(viewMatrix);
  tproj = XMMatrixTranspose(projectionMatrix);
  tLightViewMatrix = XMMatrixTranspose(light->GetViewMatrix());
  tLightProjectionMatrix = XMMatrixTranspose(light->GetProjectionMatrix());

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  dataPtr->world = tworld;// worldMatrix;
  dataPtr->view = tview;
  dataPtr->projection = tproj;
  dataPtr->lightView = tLightViewMatrix;
  dataPtr->lightProjection = tLightProjectionMatrix;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

  //Additional
  // Send light data to pixel shader
  deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  lightPtr = (LightBufferType*)mappedResource.pData;
  lightPtr->ambient = light->GetAmbientColour();
  lightPtr->diffuse = light->GetDiffuseColour();
  deviceContext->Unmap(m_LightBuffer, 0);
  bufferNumber = 0;
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer);

  // Send light data to vertex shader
  deviceContext->Map(m_LightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  lightPtr2 = (LightBufferType2*)mappedResource.pData;
  lightPtr2->position = light->GetPosition();
  lightPtr2->padding = 1.0f;
  deviceContext->Unmap(m_LightBuffer2, 0);
  bufferNumber = 1;
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer2);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture);
  // Set shader depth map texture resource.
  deviceContext->PSSetShaderResources(1, 1, &depthMap);
}

void ShadowShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);
  deviceContext->PSSetSamplers(1, 1, &m_sampleStateClamp);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}






// 80 //////////////////////////////////////////////////////////////////////////