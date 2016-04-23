////////////////////////////////////////////////////////////////////////////////
/**
* \file       Simplex2DheightShader.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "Simplex2DheightShader.h"
#include<System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


Simplex2DheightShader::Simplex2DheightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd),
                              m_matrixBuffer(nullptr), m_Simplex2DBuffer(nullptr)
{

}


Simplex2DheightShader::~Simplex2DheightShader()
{
  RELEASE_OBJECT(m_Simplex2DBuffer);
  RELEASE_OBJECT(m_matrixBuffer);
}


// -----------------------------------------------------------------------------

void Simplex2DheightShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_BUFFER_DESC simplex2DBufferDesc;

  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  ZeroMemory(&simplex2DBufferDesc, sizeof(simplex2DBufferDesc));

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
  simplex2DBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  simplex2DBufferDesc.ByteWidth = sizeof(Simplex2DBufferType);
  simplex2DBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  simplex2DBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  simplex2DBufferDesc.MiscFlags = 0;
  simplex2DBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_Simplex2DBuffer);

}

// -----------------------------------------------------------------------------

void Simplex2DheightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
                                                const XMMATRIX &worldMatrix,
                                                const XMMATRIX &viewMatrix,
                                                const XMMATRIX &projectionMatrix,
                                                float freq, float ampl, float xOff, float yOff)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* matrixPtr;
  Simplex2DBufferType* simplex2DPtr;
  unsigned int bufferNumber;
  XMMATRIX tworld, tview, tproj;


  // Transpose the matrices to prepare them for the shader.
  tworld = XMMatrixTranspose(worldMatrix);
  tview = XMMatrixTranspose(viewMatrix);
  tproj = XMMatrixTranspose(projectionMatrix);

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  matrixPtr = (MatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  matrixPtr->world = tworld;// worldMatrix;
  matrixPtr->view = tview;
  matrixPtr->projection = tproj;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);



  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_Simplex2DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the constant buffer.
  simplex2DPtr = (Simplex2DBufferType*)mappedResource.pData;

  // Copy the values into the constant buffer.
  simplex2DPtr->frequency = freq;
  simplex2DPtr->amplitude = ampl;
  simplex2DPtr->xOffset = xOff;
  simplex2DPtr->yOffset = yOff;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_Simplex2DBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_Simplex2DBuffer);

}

// -----------------------------------------------------------------------------

void Simplex2DheightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Base render function.
  BaseShader::Render(deviceContext, indexCount);
}

// -----------------------------------------------------------------------------






// 80 //////////////////////////////////////////////////////////////////////////