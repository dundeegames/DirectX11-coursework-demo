////////////////////////////////////////////////////////////////////////////////
/**
* \file       Procedural.cpp
* \brief      Procedural class
*
* \details    
*
* \author     Jiri Klic
* \version    1.0
* \date       February 2016
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

Procedural::Procedural() : m_PointMesh(nullptr), m_GeometryShader(nullptr),
                           m_SpriteBatch(nullptr), m_SpriteFont(nullptr),
                           m_TextPosition(0.0f, 20.0f), m_SphereMesh(nullptr),
                           m_SphereShader(nullptr), m_PlaneTessellShader(nullptr),
                           m_RendStateHelp(nullptr),
                           m_Terrain(nullptr), m_fBmTessellShader(nullptr),
                           m_RenderStage(FBM_STAGE), m_EffectStage(NORMAL_STAGE),
                           m_colourOverlay(0.8f, 0.4f, 0.0f)
{
  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    m_Lights[i] = nullptr;
  }

}


Procedural::~Procedural()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_PointMesh);
  DELETE_OBJECT(m_GeometryShader);
  DELETE_OBJECT(m_fBmTessellShader);
  DELETE_OBJECT(m_PlaneTessellShader);
  DELETE_OBJECT(m_SphereShader);

  DELETE_OBJECT(m_SphereMesh);
  DELETE_OBJECT(m_Terrain);  
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

void Procedural::init(HINSTANCE hinstance, HWND hwnd,
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



  m_Camera->SetPosition(40.0f, 30.0f, -50.0f);
  m_Camera->SetRotation(32.0f, -39.0f, 0.0f);


  // Create Mesh object
  m_SphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");


  m_Terrain = new TerrainMesh(m_Direct3D->GetDevice(), L"../media/waterDisplaceMap.jpg",
                              5.0f, 5.0f, 32, 32);

  
  m_SphereShader = new UberTessellShader(m_Direct3D->GetDevice(), hwnd);
  m_SphereShader->InitShader(L"shaders/uber_vs.hlsl", L"shaders/ubermarble_ps.hlsl");


  m_fBmTessellShader = new UberTessellShader(m_Direct3D->GetDevice(), hwnd);
  m_fBmTessellShader->InitShader(L"shaders/ubertessell_vs.hlsl", L"shaders/ubertessell_hs.hlsl",
                                  L"shaders/ubertessellfbm_ds.hlsl", L"shaders/ubertessellfbm_ps.hlsl");


  m_PlaneTessellShader = new UberTessellShader(m_Direct3D->GetDevice(), hwnd);
  m_PlaneTessellShader->InitShader(L"shaders/ubertessell_vs.hlsl", L"shaders/ubertessell_hs.hlsl",
                                   L"shaders/ubertessell_ds.hlsl", L"shaders/ubertessell_ps.hlsl");


  initLights();
  

  // RenderTexture, OrthoMesh and shader set for different renderTarget
  m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
  
  // ortho size and position set based on window size
  // 200x200 pixels (standard would be matching window size for fullscreen mesh
  // Position default at 0x0 centre window, to offset change values (pixel)
  m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), screenWidth, screenHeight);


  m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
  m_TextureShader->InitShader(L"shaders/texture_vs.hlsl", L"shaders/texture_ps.hlsl");


  m_tessellationLevel = 8.0f;
  m_sphereRotation = 0.0f;

}

// -----------------------------------------------------------------------------

bool Procedural::Frame()
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
    m_RenderStage = FBM_STAGE;
  }

  if (m_Input->isKeyDown(VK_F6))
  {
    m_RenderStage = SIMPLEX_STAGE;
  }

  if (m_Input->isKeyDown('P'))
  {
    m_tessellationLevel += 5.0f * m_Timer->GetTime();

    if (m_tessellationLevel > 64.0f)
    {
      m_tessellationLevel = 64.0f;
    }
  }

  if (m_Input->isKeyDown('O'))
  {
    m_tessellationLevel -= 5.0f * m_Timer->GetTime();

    if (m_tessellationLevel < 1.0f)
    {
      m_tessellationLevel = 1.0f;
    }
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

bool Procedural::Render()
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

void Procedural::initLights()
{
  m_Lights[0] = new Light;
  m_Lights[0]->SetDiffuseColour(0.25f, 0.25f, 0.25f, 1.0f);  // white
  m_Lights[0]->SetAmbientColour(0.5f, 0.5f, 0.5f, 1.0f); // gray
  m_Lights[0]->SetPosition(5.0f, 9.0f, 0.0f);
  //m_Lights[0]->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);  // no specular
  m_Lights[0]->SetSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);  // white
  m_Lights[0]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[0]);

  m_Lights[1] = new Light;
  m_Lights[1]->SetDiffuseColour(0.5f, 0.25f, 0.25f, 1.0f);  // light red
  m_Lights[1]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[1]->SetPosition(25.0f, 5.0f, 25.0f);
  //m_Lights[1]->SetSpecularColour(0.75f, 0.0f, 0.0f, 1.0f);  // red
  m_Lights[1]->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);  // no specular
  m_Lights[1]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[1]);

  m_Lights[2] = new Light;
  m_Lights[2]->SetDiffuseColour(0.25f, 0.25f, 0.5f, 1.0f);  // light blue
  m_Lights[2]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[2]->SetPosition(25.0f, 7.5f, -25.0f);
  m_Lights[2]->SetSpecularColour(0.0f, 0.0f, 0.75f, 1.0f);  // blue
  m_Lights[2]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[2]);

  m_Lights[3] = new Light;
  m_Lights[3]->SetDiffuseColour(0.0f, 0.2f, 0.0f, 1.0f);  // green
  m_Lights[3]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[3]->SetPosition(-25.0f, 8.0f, -25.0f);
  m_Lights[3]->SetSpecularColour(0.0f, 0.5f, 0.0f, 1.0f);  // green
  m_Lights[3]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[3]);

  m_Lights[4] = new Light;
  m_Lights[4]->SetDiffuseColour(0.2f, 0.2f, 0.0f, 1.0f);  // light yellow
  m_Lights[4]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[4]->SetPosition(-35.0f, 9.0f, 35.0f);
  m_Lights[4]->SetSpecularColour(0.5f, 0.5f, 0.0f, 1.0f);  // yellow
  m_Lights[4]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[4]);

  m_Lights[5] = new Light;
  m_Lights[5]->SetDiffuseColour(0.2f, 0.1f, 0.0f, 1.0f);  // orange
  m_Lights[5]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[5]->SetPosition(0.0f, 7.5f, -25.0f);
  m_Lights[5]->SetSpecularColour(0.5f, 0.25f, 0.0f, 1.0f);  // orange
  m_Lights[5]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[5]);

  m_Lights[6] = new Light;
  m_Lights[6]->SetDiffuseColour(0.2f, 0.0f, 0.2f, 1.0f);  // violet
  m_Lights[6]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[6]->SetPosition(0.0f, 8.0f, 25.0f);
  m_Lights[6]->SetSpecularColour(0.5f, 0.0f, 0.5f, 1.0f);  // violet
  m_Lights[6]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[6]);

  m_Lights[5] = new Light;
  m_Lights[5]->SetDiffuseColour(0.0f, 0.2f, 0.2f, 1.0f);  // magenta
  m_Lights[5]->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // no ambient
  m_Lights[5]->SetPosition(-5.0f, 9.0f, 0.0f);
  m_Lights[5]->SetSpecularColour(0.0f, 0.5f, 0.5f, 1.0f);  // magenta
  m_Lights[5]->SetSpecularPower(25.0);
  m_SphereShader->addLight(m_Lights[5]);


}

// -----------------------------------------------------------------------------

void Procedural::renderToTexture()
{
  

  // Set the render target to be the render to texture.
  m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

  // Clear the render to texture.
  m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), COLOUR_DEFAULT);

  drawGeometry();

}

// -----------------------------------------------------------------------------

void Procedural::renderOrthoMesh()
{
  XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
  // 
  int renderState = STANDARD;

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

void Procedural::drawGeometry()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  m_sphereRotation += 0.1f;

  //if (m_sphereRotation > 360.0f)
  //{
  //  m_sphereRotation -= 360.0f;
  //}



  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  worldMatrix = XMMatrixTranslation(0.0f, 12.0f, 0.0f);

  // Send geometry data (from mesh)
  m_PointMesh->SendData(m_Direct3D->GetDeviceContext());

  // Set shader parameters (matrices and texture)
  m_GeometryShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                        projectionMatrix, m_PointMesh->GetTexture(), m_Camera);

  // Render object (combination of mesh geometry and shader process
  m_GeometryShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());


  m_SphereShader->setCamera(m_Camera);

  // Send geometry data (from mesh)
  m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
  m_SphereShader->setTexture(m_SphereMesh->GetTexture());

  // Set Mesh position
  worldMatrix = XMMatrixMultiply(XMMatrixScaling(3.0f, 3.0f, 3.0f), XMMatrixTranslation(0.0f, 12.0f, 0.0f) );

  // Set shader parameters
  m_SphereShader->setView(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                          projectionMatrix, XMMatrixIdentity());

  // Render object (combination of mesh geometry and shader process
  m_SphereShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());


  switch (m_RenderStage)
  {
  case FBM_STAGE:

    m_fBmTessellShader->setCamera(m_Camera);
    m_fBmTessellShader->setFixedTessellation(m_tessellationLevel);

    // Send geometry data (from mesh)
    m_Terrain->SendData(m_Direct3D->GetDeviceContext());

    m_fBmTessellShader->setTexture(nullptr);
    m_fBmTessellShader->setHeightmap(nullptr);

    worldMatrix = XMMatrixScaling(20.0f, 4.0f, 20.0f);

    // Set shader parameters
    m_fBmTessellShader->setView(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                 projectionMatrix, XMMatrixIdentity());

    // Render object (combination of mesh geometry and shader process
    m_fBmTessellShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount());

    break;


  case SIMPLEX_STAGE:

    m_PlaneTessellShader->setCamera(m_Camera);
    m_PlaneTessellShader->setFixedTessellation(m_tessellationLevel);

    // Send geometry data (from mesh)
    m_Terrain->SendData(m_Direct3D->GetDeviceContext());

    m_PlaneTessellShader->setTexture(nullptr);
    m_PlaneTessellShader->setHeightmap(nullptr);

    worldMatrix = XMMatrixScaling(20.0f, 4.0f, 20.0f);

    // Set shader parameters
    m_PlaneTessellShader->setView(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                  projectionMatrix, XMMatrixIdentity());

    // Render object (combination of mesh geometry and shader process
    m_PlaneTessellShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount());

    break;

  default:
    break;

  }

}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////