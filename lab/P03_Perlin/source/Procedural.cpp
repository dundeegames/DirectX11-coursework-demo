////////////////////////////////////////////////////////////////////////////////
/**
* \file       Coursework.cpp
* \brief      Coursework class
*
* \details    
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
#include "Procedural.h"
#include <System/Common.h>
#include<System/MeshGenerator.h>

#include <sstream>
#include <iomanip>

// FUNCTIONS ///////////////////////////////////////////////////////////////////

Coursework::Coursework() : m_PointMesh(nullptr), m_GeometryShader(nullptr),
                           m_SpriteBatch(nullptr), m_SpriteFont(nullptr),
                           m_TextPosition(0.0f, 20.0f), m_Mesh(nullptr),
                           m_UberShader(nullptr), m_Cube(nullptr),
                           m_RendStateHelp(nullptr), m_PlaneMesh(nullptr),
                           m_Terrain(nullptr), m_TessellationShader(nullptr),
                           m_RenderStage(LIGHTS_STAGE), m_EffectStage(NORMAL_STAGE),
                           m_colourOverlay(0.8f, 0.4f, 0.0f)
{
  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    m_Lights[i] = nullptr;
  }

}


Coursework::~Coursework()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_PointMesh);
  DELETE_OBJECT(m_GeometryShader);
  DELETE_OBJECT(m_TessellationShader);
  DELETE_OBJECT(m_Mesh);
  DELETE_OBJECT(m_Cube);
  DELETE_OBJECT(m_PlaneMesh);
  DELETE_OBJECT(m_Terrain);
  DELETE_OBJECT(m_UberShader);
  DELETE_OBJECT(m_RendStateHelp);

  DELETE_OBJECT(m_RenderTexture);
  DELETE_OBJECT(m_TextureShader);
  DELETE_OBJECT(m_OrthoMesh);

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    DELETE_OBJECT(m_Lights[i]);
  }

}


// -----------------------------------------------------------------------------

void Coursework::init(HINSTANCE hinstance, HWND hwnd,
                      int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  m_RendStateHelp = new RenderStateHelper(m_Direct3D->GetDeviceContext());


  m_SpriteBatch = new SpriteBatch(m_Direct3D->GetDeviceContext());
  m_SpriteFont = new SpriteFont(m_Direct3D->GetDevice(), L"../media/Fonts/tahoma12.spritefont");


  // Create Mesh object
  m_PointMesh = new PointMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_GeometryShader = new GeometryShader(m_Direct3D->GetDevice(), hwnd);

  m_GeometryShader->InitShader(L"shaders/quad_vs.hlsl",
                               L"shaders/quad_gs.hlsl",
                               L"shaders/quad_ps.hlsl");



  m_Camera->SetPosition(20.0f, 20.0f, -40.0f);
  m_Camera->SetRotation(35.0f, -25.0f, 0.0f);


  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");
  m_Cube = new CubeMesh(m_Direct3D->GetDevice(), L"../media/brick1.dds");


  MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());
  m_PlaneMesh = meshGen.getPlane(50.0f, 50.0f, 50, 50);
  //m_PlaneMesh->setTexture(m_ResourceManager.getTexture(m_Direct3D->GetDevice(), L"brick1.dds"));
  m_PlaneMesh->setTexture(nullptr);
  m_PlaneMesh->setPosition(0.0f, -2.0f, 0.0f);
  m_PlaneMesh->setRotation(90.0f, 0.0f, 0.0f);


  m_Terrain = new TessellationMesh(m_Direct3D->GetDevice(), L"../media/waterDisplaceMap.jpg");

  m_TessellationShader = new TessellationShader(m_Direct3D->GetDevice(), hwnd);
  m_TessellationShader->InitShader(L"shaders/tessellation_vs.hlsl", L"shaders/tessellation_hs.hlsl",
                                   L"shaders/tessellation_ds.hlsl", L"shaders/tessellation_ps.hlsl");

  
  m_UberShader = new UberShader(m_Direct3D->GetDevice(), hwnd);
  m_UberShader->InitShader(L"shaders/uber_vs.hlsl", L"shaders/uber_ps.hlsl");

  initLights();


  // RenderTexture, OrthoMesh and shader set for different renderTarget
  m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
  
  // ortho size and position set based on window size
  // 200x200 pixels (standard would be matching window size for fullscreen mesh
  // Position default at 0x0 centre window, to offset change values (pixel)
  m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight);


  m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
  m_TextureShader->InitShader(L"shaders/texture_vs.hlsl", L"shaders/texture_ps.hlsl");

}

// -----------------------------------------------------------------------------

bool Coursework::Frame()
{
  bool result;

  result = BaseApplication::Frame();
  if (!result)
  {
    return false;
  }

  // Handle Input
  if (m_Input->isKeyDown(VK_F1) || m_Input->isKeyDown('2'))
  {
    m_EffectStage = NORMAL_STAGE;
  }

  if (m_Input->isKeyDown(VK_F2))
  {
    m_EffectStage = GRAYSCALE_STAGE;
  }

  if (m_Input->isKeyDown(VK_F3))
  {
    m_EffectStage = INVERSE_STAGE;
  }

  if (m_Input->isKeyDown(VK_F4))
  {
    m_EffectStage = COLOURED_STAGE;
  }

  if (m_Input->isKeyDown(VK_F5))
  {
    m_RenderStage = LIGHTS_STAGE;
  }

  if (m_Input->isKeyDown(VK_F6))
  {
    m_RenderStage = TESELLATION_STAGE;
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

bool Coursework::Render()
{
  if (m_EffectStage == NORMAL_STAGE)
  {
    // Reset the render target back to the original back buffer and not the render to texture anymore.
    m_Direct3D->SetBackBufferRenderTarget();

    // Clear the scene with default blue colour
    m_Direct3D->BeginScene(COLOUR_DEFAULT);

    drawGeometry();

    // Present the rendered scene to the screen.
    m_Direct3D->EndScene();

  }
  else
  {
    renderToTexture();
    renderOrthoMesh();
  }


  return true;
}

// -----------------------------------------------------------------------------

void Coursework::initLights()
{
  m_Lights[0] = new Light;
  m_Lights[0]->SetDiffuseColour(0.25f, 0.25f, 0.25f, 1.0f);  // white
  m_Lights[0]->SetAmbientColour(0.5f, 0.5f, 0.5f, 1.0f); // gray
  m_Lights[0]->SetPosition(5.0f, 5.0f, 0.0f);
  m_Lights[0]->SetSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);  // white
  m_Lights[0]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[0]);

  m_Lights[1] = new Light;
  m_Lights[1]->SetDiffuseColour(0.5f, 0.25f, 0.25f, 1.0f);  // light red
  m_Lights[1]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[1]->SetPosition(-15.0f, -1.0f, 15.0f);
  m_Lights[1]->SetSpecularColour(0.75f, 0.0f, 0.0f, 1.0f);  // red
  m_Lights[1]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[1]);

  m_Lights[2] = new Light;
  m_Lights[2]->SetDiffuseColour(0.25f, 0.25f, 0.5f, 1.0f);  // light blue
  m_Lights[2]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[2]->SetPosition(15.0f, -1.0f, 15.0f);
  m_Lights[2]->SetSpecularColour(0.0f, 0.0f, 0.75f, 1.0f);  // blue
  m_Lights[2]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[2]);

  m_Lights[3] = new Light;
  m_Lights[3]->SetDiffuseColour(0.0f, 0.2f, 0.0f, 1.0f);  // green
  m_Lights[3]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[3]->SetPosition(-15.0f, -1.0f, -15.0f);
  m_Lights[3]->SetSpecularColour(0.0f, 0.5f, 0.0f, 1.0f);  // green
  m_Lights[3]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[3]);

  m_Lights[4] = new Light;
  m_Lights[4]->SetDiffuseColour(0.2f, 0.2f, 0.0f, 1.0f);  // light yellow
  m_Lights[4]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[4]->SetPosition(15.0f, -1.0f, -15.0f);
  m_Lights[4]->SetSpecularColour(0.5f, 0.5f, 0.0f, 1.0f);  // yellow
  m_Lights[4]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[4]);

  m_Lights[5] = new Light;
  m_Lights[5]->SetDiffuseColour(0.2f, 0.1f, 0.0f, 1.0f);  // orange
  m_Lights[5]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[5]->SetPosition(0.0f, -1.0f, -15.0f);
  m_Lights[5]->SetSpecularColour(0.5f, 0.25f, 0.0f, 1.0f);  // orange
  m_Lights[5]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[5]);

  m_Lights[6] = new Light;
  m_Lights[6]->SetDiffuseColour(0.2f, 0.0f, 0.2f, 1.0f);  // violet
  m_Lights[6]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[6]->SetPosition(0.0f, -1.0f, 15.0f);
  m_Lights[6]->SetSpecularColour(0.5f, 0.0f, 0.5f, 1.0f);  // violet
  m_Lights[6]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[6]);

  m_Lights[5] = new Light;
  m_Lights[5]->SetDiffuseColour(0.0f, 0.2f, 0.2f, 1.0f);  // magenta
  m_Lights[5]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[5]->SetPosition(-5.0f, 5.0f, 0.0f);
  m_Lights[5]->SetSpecularColour(0.0f, 0.5f, 0.5f, 1.0f);  // magenta
  m_Lights[5]->SetSpecularPower(25.0);
  m_UberShader->addLight(m_Lights[5]);


}

// -----------------------------------------------------------------------------

void Coursework::renderToTexture()
{
  

  // Set the render target to be the render to texture.
  m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

  // Clear the render to texture.
  m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), COLOUR_DEFAULT);

  drawGeometry();

}

// -----------------------------------------------------------------------------

void Coursework::renderOrthoMesh()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
  int renderState;

  switch (m_EffectStage)
  {
  case GRAYSCALE_STAGE:
    renderState = GRAYSCALE;
    break;

  case INVERSE_STAGE:
    renderState = INVERSE;
    break;

  case COLOURED_STAGE:
    renderState = COLOURED;
    break;

  default:
    renderState = STATE_ERROR;
    break;
  }


  // Reset the render target back to the original back buffer and not the render to texture anymore.
  m_Direct3D->SetBackBufferRenderTarget();

  // Clear the scene with default blue colour
  m_Direct3D->BeginScene(COLOUR_DEFAULT);

  //m_RendStateHelp->SaveRasterizerState();

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  // To render ortho mesh
  // Turn off the Z buffer to begin all 2D rendering.
  m_Direct3D->TurnZBufferOff();

  m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
  m_Camera->GetBaseViewMatrix(baseViewMatrix);

  m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());
  m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix,
                                         orthoMatrix, m_RenderTexture->GetShaderResourceView(),
                                         renderState, m_colourOverlay);
  m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());


  // Save states before text - there are some edits within
  //m_SpriteBatch->Begin();

  //  std::wostringstream fpsLabel;
  //  //fpsLabel << std::setprecision(4) << L"Frame Rate: " << mFrameRate << "    Total Elapsed Time: " << gameTime.TotalGameTime();
  //  fpsLabel << std::setprecision(4) << L"Frame Rate: " << 60.123456789f << "    Total Elapsed Time: " << 151.5654789f;
  //  m_SpriteFont->DrawString(m_SpriteBatch, fpsLabel.str().c_str(), m_TextPosition);

  //m_SpriteBatch->End();


  //m_RendStateHelp->RestoreRasterizerState();


  m_Direct3D->TurnZBufferOn();

  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

}

// -----------------------------------------------------------------------------

void Coursework::drawGeometry()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  //m_GeometryShader->enableShader(m_Direct3D->GetDeviceContext());

  // Send geometry data (from mesh)
  m_PointMesh->SendData(m_Direct3D->GetDeviceContext());

  // Set shader parameters (matrices and texture)
  m_GeometryShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
    projectionMatrix, m_PointMesh->GetTexture(), m_Camera);

  // Render object (combination of mesh geometry and shader process
  m_GeometryShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());

  //Dissable Geometry Shader
  //m_GeometryShader->dissableShader(m_Direct3D->GetDeviceContext());

  m_UberShader->setCamera(m_Camera);

  // Send geometry data (from mesh)
  m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  m_UberShader->setTexture(m_Mesh->GetTexture());
  // Get Mesh position
  //worldMatrix = XMMatrixTranslation(-1.5f, 0.0f, 0.0f);
  worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
  // Set shader parameters
  m_UberShader->setView(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);

  // Render object (combination of mesh geometry and shader process
  m_UberShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

  if (m_RenderStage == LIGHTS_STAGE)
  {
    // Send geometry data (from mesh)
    m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
    m_UberShader->setTexture(m_PlaneMesh->GetTexture());
    // Get Mesh position
    worldMatrix = m_PlaneMesh->getWorldMatrix();
    // Set shader parameters
    m_UberShader->setView(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);

    // Render object (combination of mesh geometry and shader process
    m_UberShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
  }
  else
  {
    // Send geometry data (from mesh)
    m_Terrain->SendData(m_Direct3D->GetDeviceContext());
    worldMatrix = XMMatrixTranslation(0.0f, -5.0f, 0.0f);
    // Set shader parameters (matrices and texture)
    m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
      projectionMatrix, XMMatrixTranslation(0.0f, 0.0f, 0.0f),
      m_Terrain->GetTexture(), m_Camera, 64.0f);
    // Render object (combination of mesh geometry and shader process
    m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount());
  }

}

// -----------------------------------------------------------------------------







// 80 //////////////////////////////////////////////////////////////////////////