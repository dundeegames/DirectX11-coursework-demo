////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab6.cpp
* \brief      Lab6 class -
*
* \details    Render to Texture
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
#include "Lab6.h"

#include<System/Common.h>
#include<System/MeshGenerator.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab6::Lab6() : m_TMesh(nullptr), m_VertManipShader(nullptr), m_TextureShader(nullptr),
                m_Light(nullptr),m_RenderTexture(nullptr), m_OrthoMesh(nullptr)
{

}


Lab6::~Lab6()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  DELETE_OBJECT(m_TMesh);
  DELETE_OBJECT(m_VertManipShader);
  DELETE_OBJECT(m_TextureShader);

  DELETE_OBJECT(m_Light);
  DELETE_OBJECT(m_RenderTexture);
  DELETE_OBJECT(m_OrthoMesh);

}


// -----------------------------------------------------------------------------

void Lab6::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(),
                                      screenWidth, screenHeight,
                                      SCREEN_NEAR, SCREEN_DEPTH);

  MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());

  // Create Mesh object
  m_TMesh = meshGen.getPlane(50.0f, 50.0f, 50, 50);
  m_TMesh->setTexture(m_ResourceManager.getTexture(m_Direct3D->GetDevice(), L"brick1.dds"));
  m_TMesh->setPosition(0.0f, -5.0f, 25.0f);
  m_TMesh->setRotation(90.0f, 0.0f, 0.0f);


  m_VertManipShader = new VertManipShader(m_Direct3D->GetDevice(), hwnd);
  m_VertManipShader->InitShader(L"shaders/vertmanip_vs.hlsl", L"shaders/vertmanip_ps.hlsl");

  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  m_Light->SetDirection(0.0f, -1.0f, 0.0f); // point facing to
  m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  m_Light->SetSpecularPower(25.0);


  // ortho size and position set based on window size
  // 200x200 pixels (standard would be matching window size for fullscreen mesh
  // Position default at 0x0 centre window, to offset change values (pixel)
  /*m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);*/
  m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);

  m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
  m_TextureShader->InitShader(L"shaders/texture_vs.hlsl", L"shaders/texture_ps.hlsl");

}

// -----------------------------------------------------------------------------

bool Lab6::Frame()
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

bool Lab6::Render()
{
  RenderToTexture();
  RenderScene();

  return true;
}

// -----------------------------------------------------------------------------

void Lab6::RenderToTexture()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  // Set the render target to be the render to texture.
  m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

  // Clear the render to texture.
  m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, and projection matrices from the camera and d3d objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // Get Mesh position
  worldMatrix = m_TMesh->getWorldMatrix();

  // Send geometry data (from mesh)
  m_TMesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_VertManipShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_TMesh->GetTexture(), m_Light, m_Camera, m_Timer);
  // Render object (combination of mesh geometry and shader process
  m_VertManipShader->Render(m_Direct3D->GetDeviceContext(), m_TMesh->GetIndexCount());


  // Reset the render target back to the original back buffer and not the render to texture anymore.
  m_Direct3D->SetBackBufferRenderTarget();
  
}

// -----------------------------------------------------------------------------

void Lab6::RenderScene()
{

  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

  // Clear the scene with default blue colour
  m_Direct3D->BeginScene(COLOUR_DEFAULT);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // Get Mesh position
  worldMatrix = m_TMesh->getWorldMatrix();

  // Send geometry data (from mesh)
  m_TMesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_VertManipShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_TMesh->GetTexture(), m_Light, m_Camera, m_Timer);
  // Render object (combination of mesh geometry and shader process
  m_VertManipShader->Render(m_Direct3D->GetDeviceContext(), m_TMesh->GetIndexCount());


  m_Direct3D->GetWorldMatrix(worldMatrix);
  // To render ortho mesh
  // Turn off the Z buffer to begin all 2D rendering.
  m_Direct3D->TurnZBufferOff();

  m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
  m_Camera->GetBaseViewMatrix(baseViewMatrix);

  m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());
  m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
  m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

  m_Direct3D->TurnZBufferOn();

  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();
}

// -----------------------------------------------------------------------------









// 80 //////////////////////////////////////////////////////////////////////////