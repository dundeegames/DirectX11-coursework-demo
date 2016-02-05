////////////////////////////////////////////////////////////////////////////////
/**
* \file       TessellationShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "tessellationshader.h"
#include  <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


TessellationShader::TessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{

}

TessellationShader::~TessellationShader()
{
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_domainSampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_textureMatrixBuffer);
  RELEASE_OBJECT(m_layout);
  RELEASE_OBJECT(m_tessellationBuffer);

  //Release base shader components
  BaseShader::~BaseShader();
}

// -----------------------------------------------------------------------------

void TessellationShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_BUFFER_DESC textureMatrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC tessellationBufferDesc;

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
  //m_device->CreateSamplerState(&samplerDesc, &m_domainSampleState);

  // Setup light buffer
  // Setup the description of the light dynamic constant buffer that is in the pixel shader.
  // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
  tessellationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  tessellationBufferDesc.ByteWidth = sizeof(TessellationBufferType);
  tessellationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  tessellationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  tessellationBufferDesc.MiscFlags = 0;
  tessellationBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&tessellationBufferDesc, NULL, &m_tessellationBuffer);
}

// -----------------------------------------------------------------------------

void TessellationShader::InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
  // InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
  InitShader(vsFilename, psFilename);

  // Load other required shaders.
  loadHullShader(hsFilename);
  loadDomainShader(dsFilename);
}

// -----------------------------------------------------------------------------

void TessellationShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
                                              const XMMATRIX &worldMatrix,
                                              const XMMATRIX &viewMatrix,
                                              const XMMATRIX &projectionMatrix,
                                              const XMMATRIX &textureMatrix,
                                              ID3D11ShaderResourceView* heightmap,
                                              Camera* camera, float tessFactor)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  TextureMatrixBufferType* textureMatrixPtr;
  TessellationBufferType* tessellationPtr;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj, texMatrix;


  // Transpose the matrices to prepare them for the shader.
  texMatrix = XMMatrixTranspose(worldMatrix);

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

  // if tessellating:
  //// Now set the constant buffer in the vertex shader with the updated values.
  //deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
  // else
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

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

  //Additional
  // Send light data to pixel shader
  deviceContext->Map(m_tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  tessellationPtr = (TessellationBufferType*)mappedResource.pData;
  tessellationPtr->tessellationFactor = tessFactor;
  tessellationPtr->cameraPosition = camera->GetPosition();
  deviceContext->Unmap(m_tessellationBuffer, 0);
  bufferNumber = 0;
  deviceContext->HSSetConstantBuffers(bufferNumber, 1, &m_tessellationBuffer);

  // Set shader texture resource in the domain shader.
  deviceContext->DSSetShaderResources(0, 1, &heightmap);
}

// -----------------------------------------------------------------------------

void TessellationShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the sampler state in the domain shader.
  //deviceContext->DSSetSamplers(0, 1, &m_domainSampleState);
  deviceContext->DSSetSamplers(0, 1, &m_sampleState);

  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}


// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////