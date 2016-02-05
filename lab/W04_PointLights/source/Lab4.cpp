////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab4.cpp
* \brief      Lab4 class -
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
#include "Lab4.h"
#include<System/Common.h>
#include<System/MeshGenerator.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab4::Lab4() : m_PlaneMesh(nullptr), m_Mesh(nullptr),
                m_PointLightShader(nullptr), m_Light(nullptr)
{

}


Lab4::~Lab4()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_Mesh);
  DELETE_OBJECT(m_PointLightShader);
  DELETE_OBJECT(m_PlaneMesh);

}


// -----------------------------------------------------------------------------

void Lab4::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());

  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_PlaneMesh = meshGen.getPlane(100.0f, 100.0f, 50, 50);
  m_PlaneMesh->setTexture(m_ResourceManager.getTexture(m_Direct3D->GetDevice(), L"brick1.dds"));
  m_PlaneMesh->setPosition(0.0f, -2.0f, 3.0f);
  m_PlaneMesh->setRotation(90.0f, 0.0f, 0.0f);
  
  m_PointLightShader = new PointLightShader(m_Direct3D->GetDevice(), hwnd);
  m_PointLightShader->InitShader(L"shaders/ptlight_vs.hlsl", L"shaders/ptlight_ps.hlsl");
  
  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetDiffuseColour(0.0f, 0.0f, 0.0f, 0.0f);  // none
  //m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red

  m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // none
  //m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  //m_Light->SetAmbientColour(0.25f, 0.5f, 0.25f, 1.0f);  // green
  m_Light->SetDirection(0.0f, 1.0f, 0.0f); // point facing to
  m_Light->SetPosition(0.0f, 0.125f, 0.0f);  // 10 points above the center

  //m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  ////m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);  // blue
  //m_Light->SetSpecularPower(25.0);
  
}

// -----------------------------------------------------------------------------

bool Lab4::Frame()
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

bool Lab4::Render()
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  // Clear the scene with default blue colour
  m_Direct3D->BeginScene(COLOUR_DEFAULT);

  // Generate the view matrix based on the camera's position.
  m_Camera->Update();

  // Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  //// Send geometry data (from mesh)
  //m_Mesh->SendData(m_Direct3D->GetDeviceContext());
  //// Set shader parameters (matrices and texture)
  //m_PointLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
  //                                          m_Mesh->GetTexture(), m_Light, m_Camera);
  //// Render object (combination of mesh geometry and shader process
  //m_PointLightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());


  // Get Mesh position
  worldMatrix = m_PlaneMesh->getWorldMatrix();

  // Send geometry data (from mesh)
  m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_PointLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_PlaneMesh->GetTexture(), m_Light, m_Camera);
  // Render object (combination of mesh geometry and shader process
  m_PointLightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
  


  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////