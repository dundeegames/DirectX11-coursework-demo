////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab6s.h
* \brief      Lab6s class -
*
* \details    Render to Texture, based on Lab3 Specular
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
#include "Lab6s.h"

#include<System/Common.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab6s::Lab6s() : m_Mesh(nullptr), m_Shader(nullptr), m_Light(nullptr)
{

}


Lab6s::~Lab6s()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  DELETE_OBJECT(m_Mesh);
  DELETE_OBJECT(m_Shader);
  DELETE_OBJECT(m_AnotherCamera);

}


// -----------------------------------------------------------------------------

void Lab6s::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create the another camera object.
  m_AnotherCamera = new Camera();
  // Initialize a base view matrix with the camera for 2D user interface rendering.
  m_AnotherCamera->SetPosition(0.0f, 0.0f, -5.0f);
  m_AnotherCamera->Update();



  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_Shader = new SpecularShader(m_Direct3D->GetDevice(), hwnd);
  m_Shader->InitShader(L"shaders/specular_vs.hlsl", L"shaders/specular_ps.hlsl");

  m_Light = new Light;  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  m_Light->SetDirection(0.0f, 1.0f, 0.0f); // point facing to
  m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  m_Light->SetSpecularPower(25.0);


  // RenderTexture, OrthoMesh and shader set for different renderTarget
  m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
  // ortho size and position set based on window size
  // 200x200 pixels (standard would be matching window size for fullscreen mesh
  // Position default at 0x0 centre window, to offset change values (pixel)
  m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);
  //m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), 150, 100, -125, 100);

  m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
  m_TextureShader->InitShader(L"shaders/texture_vs.hlsl", L"shaders/texture_ps.hlsl");

}

// -----------------------------------------------------------------------------

bool Lab6s::Frame()
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

bool Lab6s::Render()
{
  RenderToTexture();
  RenderScene();

  return true;
}

// -----------------------------------------------------------------------------

void Lab6s::RenderToTexture()
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
  m_AnotherCamera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());

  m_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
                                m_Mesh->GetTexture(), m_Light, m_AnotherCamera, false);
  // Render object (combination of mesh geometry and shader process
  m_Shader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

  // Reset the render target back to the original back buffer and not the render to texture anymore.
  m_Direct3D->SetBackBufferRenderTarget();
}

// -----------------------------------------------------------------------------

void Lab6s::RenderScene()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

  // Clear the scene. (default blue colour)
  m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // Send geometry data (from mesh)
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_Shader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, m_Camera);
  // Render object (combination of mesh geometry and shader process
  m_Shader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

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