////////////////////////////////////////////////////////////////////////////////
/**
* \file       VertManipShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "VertManipShader.h"

#include<System/Common.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


VertManipShader::VertManipShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
  time = 0.0f;
  speed = 0.0f;
}


VertManipShader::~VertManipShader()
{
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_layout);
  RELEASE_OBJECT(m_lightBuffer);
  RELEASE_OBJECT(m_cameraBuffer);

  //Release base shader components
  BaseShader::~BaseShader();
}


// -----------------------------------------------------------------------------

void VertManipShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC specularBufferDesc;
  D3D11_BUFFER_DESC cameraBufferDesc;
  //D3D11_BUFFER_DESC timeBufferDesc;

  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  ZeroMemory(&samplerDesc, sizeof(samplerDesc));
  ZeroMemory(&specularBufferDesc, sizeof(specularBufferDesc));
  ZeroMemory(&cameraBufferDesc, sizeof(cameraBufferDesc));
  //ZeroMemory(&timeBufferDesc, sizeof(timeBufferDesc));

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

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
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

  // Setup specular buffer
  // Setup the description of the light dynamic constant buffer that is in the pixel shader.
  // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER
  // or CreateBuffer will fail.
  specularBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  specularBufferDesc.ByteWidth = sizeof(SpecularBufferType);
  specularBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  specularBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  specularBufferDesc.MiscFlags = 0;
  specularBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
  m_device->CreateBuffer(&specularBufferDesc, NULL, &m_lightBuffer);

  // Setup the description of the dynamic camera constant buffer that is in the vertex shader.
  cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  cameraBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cameraBufferDesc.MiscFlags = 0;
  cameraBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
  m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);

}

// -----------------------------------------------------------------------------

void VertManipShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
  const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix,
  const XMMATRIX &projectionMatrix,
  ID3D11ShaderResourceView* texture, Light* light, Camera* camera, Timer* timer)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  CameraBufferType* camPtr;
  SpecularBufferType* specularPtr;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj;

  ZeroMemory(&mappedResource, sizeof(mappedResource));

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

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


  // ??? Camera Buffer ???
  // ---------------------------
  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  camPtr = (CameraBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  camPtr->position = camera->GetPosition();// worldMatrix;
  speed = 0.1f;
  time += ( speed * timer->GetTime() );
  camPtr->time = time;
  // Unlock the constant buffer.
  deviceContext->Unmap(m_cameraBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 1;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
  // ---------------------------



  //Additional
  // Send specular light data to pixel shader

  // Lock the light constant buffer so it can be written to
  deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  // Get a pointer to the data in the constant buffer
  specularPtr = (SpecularBufferType*)mappedResource.pData;

  // Copy light data into the buffer
  specularPtr->diffuse = light->GetDiffuseColour();
  specularPtr->ambient = light->GetAmbientColour();
  specularPtr->direction = light->GetDirection();
  specularPtr->specularPower = light->GetSpecularPower();
  specularPtr->specular = light->GetSpecularColour();

  // Unlock the buffer
  deviceContext->Unmap(m_lightBuffer, 0);
  // Set buffer number (register value)
  bufferNumber = 0;
  // Set the constant buffer in the pixel shader
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture);
}

// -----------------------------------------------------------------------------

void VertManipShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}

// -----------------------------------------------------------------------------










// 80 //////////////////////////////////////////////////////////////////////////