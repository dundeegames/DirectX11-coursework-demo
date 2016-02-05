////////////////////////////////////////////////////////////////////////////////
/**
* \file       MultiLightShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "MultiLightShader.h"

#include <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


MultiLightShader::MultiLightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{

}


MultiLightShader::~MultiLightShader()
{
  // Release the MultiLightShader components
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_layout);
  RELEASE_OBJECT(m_lightBuffer);

  //Release BaseShader components
  BaseShader::~BaseShader();
}


// -----------------------------------------------------------------------------

void MultiLightShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC pointBufferDesc;
  D3D11_BUFFER_DESC cameraBufferDesc;

  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  ZeroMemory(&samplerDesc, sizeof(samplerDesc));
  ZeroMemory(&pointBufferDesc, sizeof(pointBufferDesc));
  ZeroMemory(&cameraBufferDesc, sizeof(cameraBufferDesc));

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

  // Setup specular buffer
  // Setup the description of the light dynamic constant buffer that is in the pixel shader.
  // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER
  // or CreateBuffer will fail.
  pointBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  pointBufferDesc.ByteWidth = sizeof(PointBufferType);
  pointBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  pointBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  pointBufferDesc.MiscFlags = 0;
  pointBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer
  // from within this class.
  m_device->CreateBuffer(&pointBufferDesc, NULL, &m_lightBuffer);

  //// Setup the description of the dynamic camera constant buffer that is in the vertex shader.
  //cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  //cameraBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  //cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  //cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  //cameraBufferDesc.MiscFlags = 0;
  //cameraBufferDesc.StructureByteStride = 0;

  //// Create the constant buffer pointer so we can access the vertex shader constant buffer
  //// from within this class.
  //m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);

}


void MultiLightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
                                            const XMMATRIX &worldMatrix,
                                            const XMMATRIX &viewMatrix,
                                            const XMMATRIX &projectionMatrix,
                                            ID3D11ShaderResourceView* texture,
                                            std::vector<Light*>& lights)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  PointBufferType* pointPtr;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj;
  XMFLOAT3 tempPosition;

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

  //// Camera Buffer -------------
  //// Lock the constant buffer so it can be written to.
  //result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  //// Get a pointer to the data in the constant buffer.
  //camPtr = (CameraBufferType*)mappedResource.pData;

  //// Copy the matrices into the constant buffer.
  //camPtr->position = camera->GetPosition();// worldMatrix;

  //// Unlock the constant buffer.
  //deviceContext->Unmap(m_cameraBuffer, 0);

  //// Set the position of the constant buffer in the vertex shader.
  //bufferNumber = 1;

  //// Now set the constant buffer in the vertex shader with the updated values.
  //deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
  //// ---------------------------



  //Additional
  // Send specular light data to pixel shader

  // Lock the light constant buffer so it can be written to
  deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  // Get a pointer to the data in the constant buffer
  pointPtr = (PointBufferType*)mappedResource.pData;

  // Copy light data into the buffer
  //pointPtr->diffuse = light1->GetDiffuseColour();
  //pointPtr->ambient = light1->GetAmbientColour();
  //tempPosition = light1->GetPosition();
  //pointPtr->position = XMFLOAT4(tempPosition.x, tempPosition.y, tempPosition.z, 1.0f);


  int numLights = max(lights.size(), MAX_LIGHTS);

  for (int i = 0; i < numLights; i++)
  {
    pointPtr->diffuse[i] = lights[i]->GetDiffuseColour();
    //pointPtr->ambient[0] = light1->GetAmbientColour();
    tempPosition = lights[i]->GetPosition();
    pointPtr->position[i] = XMFLOAT4(tempPosition.x, tempPosition.y, tempPosition.z, 1.0f);
  }

  

  //pointPtr->diffuse[1] = light2->GetDiffuseColour();
  ////pointPtr->ambient[1] = light2->GetAmbientColour();
  //tempPosition = light2->GetPosition();
  //pointPtr->position[1] = XMFLOAT4(tempPosition.x, tempPosition.y, tempPosition.z, 1.0f);

  // Unlock the buffer
  deviceContext->Unmap(m_lightBuffer, 0);
  // Set buffer number (register value)
  bufferNumber = 0;
  // Set the constant buffer in the pixel shader
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture);
}

void MultiLightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}






// 80 //////////////////////////////////////////////////////////////////////////







