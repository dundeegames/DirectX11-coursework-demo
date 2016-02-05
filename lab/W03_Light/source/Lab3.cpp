////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab3.cpp
* \brief      Lab3 class -
*
* \details    Lighting
*
* \author     Jiri Klic
* \version    1.0
* \date       Spetember 2015
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
#include "Lab3.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


//Lab3::Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
//{
//  // Create Mesh object
//  //m_Mesh = new TriangleMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
//  //m_Mesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/bunny.png");
//  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/bunny.png");
//
//  m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);
//
//  m_Light = new Light;
//
//  m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);
//  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 0.25f);
//  m_Light->SetDirection(0.5f, 0.5f, 0.0f); // point facing to
//
//
//}

Lab3::Lab3()
{
  m_Mesh = nullptr;
  m_LightShader = nullptr;
  m_Light = nullptr;
}


Lab3::~Lab3()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  if (m_Mesh)
  {
    delete m_Mesh;
    m_Mesh = 0;
  }

  if (m_LightShader)
  {
    delete m_LightShader;
    m_LightShader = 0;
  }
}


// -----------------------------------------------------------------------------

void Lab3::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create Mesh object
  //m_Mesh = new TriangleMesh(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png");
  //m_Mesh = new QuadMesh(m_Direct3D->GetDevice(), L"../res/bunny.png");
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/bunny.png");
  m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);
  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);
  //m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 0.25f);
  m_Light->SetAmbientColour(0.25f, 0.5f, 0.25f, 0.25f);
  m_Light->SetDirection(0.5f, 0.5f, 0.0f); // point facing to
  
}

// -----------------------------------------------------------------------------

bool Lab3::Frame()
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

bool Lab3::Render()
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
  m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(),worldMatrix, viewMatrix, projectionMatrix,
                                       m_Mesh->GetTexture(), m_Light);
  // Render object (combination of mesh geometry and shader process
  m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////