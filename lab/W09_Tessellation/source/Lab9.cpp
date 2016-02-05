////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab9.h
* \brief      Lab9 class -
*
* \details    Tessellation
*
* \author     Jiri Klic
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
#include "Lab9.h"
#include<System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////

const XMFLOAT2 Lab9::m_TextureTranformSpeed = XMFLOAT2(-0.1f, 0.05f);


Lab9::Lab9() : m_Trinagle(nullptr), m_TessellationShader(nullptr), m_TexturePosition(0.0f, 0.0f)
{

}


Lab9::~Lab9()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_Trinagle);
  DELETE_OBJECT(m_TessellationShader);

}


// -----------------------------------------------------------------------------

void Lab9::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create Mesh object
  m_Trinagle = new TessellationMesh(m_Direct3D->GetDevice(), L"../media/waterDisplaceMap.jpg");

  m_TessellationShader = new TessellationShader(m_Direct3D->GetDevice(), hwnd);
  m_TessellationShader->InitShader(L"shaders/tessellation_vs.hlsl", L"shaders/tessellation_hs.hlsl",
                                   L"shaders/tessellation_ds.hlsl", L"shaders/tessellation_ps.hlsl");
  
  m_Camera->SetPosition(20.0f, 20.0f, -40.0f);
  m_Camera->SetRotation(35.0f, -25.0f, 0.0f);

}

// -----------------------------------------------------------------------------

bool Lab9::Frame()
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

bool Lab9::Render()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, textureMatrix;
  float dt = m_Timer->GetTime();

  // Clear the scene with default blue colour
  m_Direct3D->BeginScene(COLOUR_DEFAULT);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);


  m_TexturePosition.x += m_TextureTranformSpeed.x * dt;
  m_TexturePosition.y += m_TextureTranformSpeed.y * dt;
  textureMatrix = XMMatrixTranslation(m_TexturePosition.x, m_TexturePosition.y, 0.0f);

  // Send geometry data (from mesh)
  m_Trinagle->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                            projectionMatrix, textureMatrix,
                                            m_Trinagle->GetTexture(), m_Camera, 64.0f);

  // Render object (combination of mesh geometry and shader process
  m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_Trinagle->GetIndexCount());
  


  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////