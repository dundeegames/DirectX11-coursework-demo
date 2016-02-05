////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab3s.cpp
* \brief      Lab3s class -
*
* \details    Lighting
*
* \author     Jiri Klic
* \version    1.0
* \date       Spetember 2015
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
#include "Lab3s.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab3s::Lab3s()
{
  m_Mesh = nullptr;
  m_SpecularShader = nullptr;
  m_Light = nullptr;
}


Lab3s::~Lab3s()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  if (m_Mesh)
  {
    delete m_Mesh;
    m_Mesh = 0;
  }

  if (m_SpecularShader)
  {
    delete m_SpecularShader;
    m_SpecularShader = 0;
  }
}


// -----------------------------------------------------------------------------

void Lab3s::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_SpecularShader = new SpecularShader(m_Direct3D->GetDevice(), hwnd);
  m_SpecularShader->InitShader(L"shaders/specular_vs.hlsl", L"shaders/specular_ps.hlsl");

  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetDiffuseColour(0.0f, 0.0f, 0.0f, 0.0f);  // none
  //m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  
  //m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 0.0f); // none
  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  //m_Light->SetAmbientColour(0.25f, 0.5f, 0.25f, 1.0f);  // green
  m_Light->SetDirection(0.0f, 1.0f, 0.0f); // point facing to

  //m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);  // blue
  m_Light->SetSpecularPower(25.0);
}

// -----------------------------------------------------------------------------

bool Lab3s::Frame()
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

bool Lab3s::Render()
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
  //worldMatrix = XMMatrixTranslation(-3.0f, 0.0f, 0.0f);
  //m_Mesh->setTexture(L"../res/bunny.png");

  // Send geometry data (from mesh)
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_SpecularShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
                                       m_Mesh->GetTexture(), m_Light, m_Camera);
  // Render object (combination of mesh geometry and shader process
  m_SpecularShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////