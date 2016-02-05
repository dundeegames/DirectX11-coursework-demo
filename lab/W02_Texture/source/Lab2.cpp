////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab2.cpp
* \brief      Lab2 class - Texturing
*
* \details    Texturing
*
* \author     Jiri Klic
* \version    1.0
* \date       September 2015
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
#include "Lab2.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab2::Lab2()
{


}


Lab2::~Lab2()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  if (m_Mesh)
  {
    delete m_Mesh;
    m_Mesh = 0;
  }

  if (m_TextureShader)
  {
    delete m_TextureShader;
    m_TextureShader = 0;
  }
}


// -----------------------------------------------------------------------------

void Lab2::init(HINSTANCE hinstance, HWND hwnd,
  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);
    // Create Mesh object
  m_TMesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
    m_Mesh = new QuadMesh(m_Direct3D->GetDevice(), L"../media/bunny.png");
  
    m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
}

// -----------------------------------------------------------------------------

bool Lab2::Frame()
{
  bool result;

  result = BaseApplication::Frame();
  if (!result)
  {
    return false;
  }

  // Render the graphics.
  result = Render();
  if (!result)
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool Lab2::Render()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  // Clear the scene. (default blue colour)
  m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // Move quad first to the left
  // XMMatrixTranslation generates new Matrix based on identity matrix
  // to edit orginal worldMatrix we use XMMatrixMultiply.
  worldMatrix = XMMatrixTranslation(-3.0f, 0.0f, 0.0f);
  
  //m_Mesh->setTexture(L"media/bunny.png");

  // Send geometry data (from mesh)
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture());
  // Render object (combination of mesh geometry and shader process
  m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

  // Render second Quad
  worldMatrix = XMMatrixTranslation(3.0f, 0.0f, 0.0f);
  //m_Mesh->setTexture(L"media/DefaultDiffuse.png");

  // Send geometry data (from mesh)
  m_TMesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_TMesh->GetTexture());
  // Render object (combination of mesh geometry and shader process
  m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_TMesh->GetIndexCount());





  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------




// 80 //////////////////////////////////////////////////////////////////////////