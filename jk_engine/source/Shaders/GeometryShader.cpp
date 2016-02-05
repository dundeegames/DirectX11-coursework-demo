////////////////////////////////////////////////////////////////////////////////
/**
* \file       GeometryShader.cpp
* \brief      GeometryShader class - 
*
* \details     
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
#include "GeometryShader.h"
#include <System/Common.h>




// FUNCTIONS ///////////////////////////////////////////////////////////////////


GeometryShader::GeometryShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{

}


GeometryShader::~GeometryShader()
{
  RELEASE_OBJECT(m_sampleState);
  RELEASE_OBJECT(m_matrixBuffer);
  RELEASE_OBJECT(m_cameraBuffer);
  RELEASE_OBJECT(m_layout);

  //Release base shader components
  BaseShader::~BaseShader();
}


// -----------------------------------------------------------------------------

void GeometryShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC cameraBufferDesc;

  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  ZeroMemory(&samplerDesc, sizeof(samplerDesc));
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

void GeometryShader::InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
  // InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
  InitShader(vsFilename, psFilename);

  // Load other required shaders.
  loadGeometryShader(gsFilename);
}

// -----------------------------------------------------------------------------

void GeometryShader::enableShader(ID3D11DeviceContext* deviceContext)
{
  // if geometry shader is not null then set GS
  if (m_geometryShader)
  {
    deviceContext->GSSetShader(m_geometryShader, NULL, 0);
  }
}

// -----------------------------------------------------------------------------

void GeometryShader::dissableShader(ID3D11DeviceContext* deviceContext)
{
  deviceContext->GSSetShader(NULL, NULL, 0);
}

// -----------------------------------------------------------------------------

void GeometryShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix,
                      const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
                      ID3D11ShaderResourceView* texture, Camera* camera)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  CameraBufferType* camPtr;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj;

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
  deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


  // CAMERA: ---------
  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  camPtr = (CameraBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  camPtr->position = camera->GetPosition();// worldMatrix;
  camPtr->up = camera->GetUp();

  // Unlock the constant buffer.
  deviceContext->Unmap(m_cameraBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 1;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);



}

// -----------------------------------------------------------------------------

void GeometryShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}

// -----------------------------------------------------------------------------












// 80 //////////////////////////////////////////////////////////////////////////