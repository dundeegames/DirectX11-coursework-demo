////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab8.cpp
* \brief      Lab8 class -
*
* \details    Post Processing - Box Blur
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
#include "Lab8.h"

#include<System/Common.h>
#include<System/MeshGenerator.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab8::Lab8() : m_Mesh(nullptr), m_SpecularShader(nullptr), m_TextureShader(nullptr),
                m_BoxBlurShader(nullptr),m_Light(nullptr), m_RenderTexture1(nullptr),
                m_RenderTexture2(nullptr), m_OrthoMesh(nullptr), m_Cube(nullptr),
                m_AnotherCamera(nullptr)
{

}


Lab8::~Lab8()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  DELETE_OBJECT(m_Mesh);
  DELETE_OBJECT(m_Cube);
  DELETE_OBJECT(m_SpecularShader);
  DELETE_OBJECT(m_BoxBlurShader);
  DELETE_OBJECT(m_TextureShader);

  DELETE_OBJECT(m_Light);
  DELETE_OBJECT(m_RenderTexture1);
  DELETE_OBJECT(m_RenderTexture2);
  DELETE_OBJECT(m_OrthoMesh);
  DELETE_OBJECT(m_AnotherCamera);

}


// -----------------------------------------------------------------------------

void Lab8::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create the another camera object.
  m_AnotherCamera = new Camera();
  // Initialize a base view matrix with the camera for 2D user interface rendering.
  m_AnotherCamera->SetPosition(4.5f, 1.0f, -4.0f);
  m_AnotherCamera->SetRotation(25.0f, -45.0f, 0.0f);
  m_AnotherCamera->Update();


  m_scrSizeBuf.screenWidth = screenWidth;
  m_scrSizeBuf.screenHeight = screenHeight;

  m_RenderTexture1 = new RenderTexture(m_Direct3D->GetDevice(),
                                      (screenWidth / 2), (screenHeight / 2),
                                      SCREEN_NEAR, SCREEN_DEPTH);

  m_RenderTexture2 = new RenderTexture(m_Direct3D->GetDevice(),
                                      screenWidth, screenHeight,
                                      SCREEN_NEAR, SCREEN_DEPTH);

  //m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(),
  //                            screenWidth / 2, screenHeight / 2,
  //                            -(screenWidth / 4), (screenHeight / 4));

  // Full screen
  m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight);


  //MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());

  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");
  m_Cube = new CubeMesh(m_Direct3D->GetDevice(), L"../media/brick1.dds");



  m_SpecularShader = new SpecularShader(m_Direct3D->GetDevice(), hwnd);
  m_SpecularShader->InitShader(L"shaders/specular_vs.hlsl", L"shaders/specular_ps.hlsl");

  m_BoxBlurShader = new BoxBlurShader(m_Direct3D->GetDevice(), hwnd);
  m_BoxBlurShader->InitShader(L"shaders/boxBlur_vs.hlsl", L"shaders/boxBlur_ps.hlsl");


  m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
  m_TextureShader->InitShader(L"shaders/texture_vs.hlsl", L"shaders/texture_ps.hlsl");


  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetDiffuseColour(0.0f, 0.0f, 0.0f, 0.0f);  // none
  //m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  
  //m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 0.0f); // none
  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  //m_Light->SetAmbientColour(0.25f, 0.5f, 0.25f, 1.0f);  // green
  m_Light->SetDirection(0.0f, 1.0f, 0.0f); // point facing to

  m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);  // blue
  m_Light->SetSpecularPower(25.0);
}

// -----------------------------------------------------------------------------

bool Lab8::Frame()
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

bool Lab8::Render()
{
  RenderToTexture();

  // To render ortho mesh
  // Turn off the Z buffer to begin all 2D rendering.
  m_Direct3D->TurnZBufferOff();

  ApplyBoxBlur();
  RenderScene();

  m_Direct3D->TurnZBufferOn();

  return true;
}

// -----------------------------------------------------------------------------

void Lab8::RenderToTexture()
{
  // Set the render target to be the render to texture.
  m_RenderTexture1->SetRenderTarget(m_Direct3D->GetDeviceContext());

  // Clear the render to texture.
  m_RenderTexture1->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.25f, 0.5f, 0.25f, 1.0f);

  //DrawGeometry(m_AnotherCamera);
  DrawGeometry(m_Camera);

  // Reset the render target back to the original back buffer and not the render to texture anymore.
  m_Direct3D->SetBackBufferRenderTarget();

  // important to reset viewport after downsampling
  m_Direct3D->ResetViewport();
}

// -----------------------------------------------------------------------------

void Lab8::ApplyBoxBlur()
{
  XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

  // Set the render target to be the render to texture.
  m_RenderTexture2->SetRenderTarget(m_Direct3D->GetDeviceContext());

  // Clear the render to texture.
  m_RenderTexture2->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

  m_Direct3D->GetWorldMatrix(worldMatrix);

  m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
  m_Camera->GetBaseViewMatrix(baseViewMatrix);

  m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());
  m_BoxBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix,
    baseViewMatrix, orthoMatrix, m_RenderTexture1->GetShaderResourceView(), &m_scrSizeBuf);

  m_BoxBlurShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

}

// -----------------------------------------------------------------------------

void Lab8::RenderScene()
{
  XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

  // Reset the render target back to the original back buffer and not the render to texture anymore.
  m_Direct3D->SetBackBufferRenderTarget();

  // Clear the scene. (default blue colour)
  m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);
  
  m_Direct3D->GetWorldMatrix(worldMatrix);

  //DrawGeometry(m_Camera);

  // To render ortho mesh
  // Turn off the Z buffer to begin all 2D rendering.
  m_Direct3D->TurnZBufferOff();

  m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
  m_Camera->GetBaseViewMatrix(baseViewMatrix);

  m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());
  m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix,
                      baseViewMatrix, orthoMatrix, m_RenderTexture1->GetShaderResourceView());
  
  m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

  m_Direct3D->TurnZBufferOn();

  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();
}

// -----------------------------------------------------------------------------

void Lab8::DrawGeometry(Camera* camera)
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  // Generate the view matrix based on the camera's position.
  camera->Update();

  // Get the world, view, and projection matrices from the camera and d3d objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);
  //projectionMatrix = m_RenderTexture1->GetProjectionMatrix();


  // Get Mesh position
  worldMatrix = XMMatrixTranslation(-1.5f, 0.0f, 0.0f);

  // Send geometry data (from mesh)
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_SpecularShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_Mesh->GetTexture(), m_Light, camera);
  // Render object (combination of mesh geometry and shader process
  m_SpecularShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());


  worldMatrix = XMMatrixTranslation(1.5f, 0.0f, 0.0f);

  // Send geometry data (from mesh)
  m_Cube->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_SpecularShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_Cube->GetTexture(), m_Light, camera);
  // Render object (combination of mesh geometry and shader process
  m_SpecularShader->Render(m_Direct3D->GetDeviceContext(), m_Cube->GetIndexCount());

}


// 80 //////////////////////////////////////////////////////////////////////////