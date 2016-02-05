////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab4m.cpp
* \brief      Lab4m class -
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
#include "Lab4m.h"
#include<System/Common.h>
#include<System/MeshGenerator.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab4m::Lab4m() : m_PlaneMesh(nullptr), m_Mesh(nullptr),
                  m_MultiLightShader(nullptr)
                  //, m_Light1(nullptr), m_Light2(nullptr)
{

}


Lab4m::~Lab4m()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_Mesh);
  DELETE_OBJECT(m_MultiLightShader);
  DELETE_OBJECT(m_PlaneMesh);

  //? Delete lights
  int lightSize = m_Lights.size();
  for (int i = 0; i < lightSize; i++)
  {
    DELETE_OBJECT(m_Lights[i]);
  }

}


// -----------------------------------------------------------------------------

void Lab4m::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());

  // Create Mesh object
  m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_PlaneMesh = meshGen.getPlane(50.0f, 50.0f, 50, 50);
  m_PlaneMesh->setTexture(m_ResourceManager.getTexture(m_Direct3D->GetDevice(), L"brick1.dds"));
  m_PlaneMesh->setPosition(0.0f, -2.0f, 0.0f);
  m_PlaneMesh->setRotation(90.0f, 0.0f, 0.0f);
  
  m_MultiLightShader = new MultiLightShader(m_Direct3D->GetDevice(), hwnd);
  m_MultiLightShader->InitShader(L"shaders/mlights_vs.hlsl", L"shaders/mlights_ps.hlsl");
  
  Light* Light1 = new Light();
  
  //m_Light1->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light1->SetDiffuseColour(0.0f, 0.0f, 0.0f, 0.0f);  // none
  //m_Light1->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  Light1->SetDiffuseColour(0.5f, 1.0f, 0.5f, 1.0f);  // green

  Light1->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // none
  //m_Light1->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  //m_Light1->SetAmbientColour(0.25f, 0.5f, 0.25f, 1.0f);  // green
  Light1->SetDirection(0.0f, 1.0f, 0.0f); // point facing to
  Light1->SetPosition(5.0f, -1.0f, 10.0f);  // 10 points above the center

  m_Lights.push_back(Light1);

  Light* Light2 = new Light();

  Light2->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //Light2->SetDiffuseColour(0.5f, 0.5f, 1.0f, 1.0f);  // blue
  //Light2->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  //m_Light2->SetDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);  // none

  Light2->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // none
  Light2->SetPosition(-5.0f, -1.0f, 10.0f);
  
  m_Lights.push_back(Light2);


  Light* Light3 = new Light();

  Light3->SetDiffuseColour(0.5f, 0.5f, 1.0f, 1.0f);  // blue
  Light3->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // none
  Light3->SetPosition(-5.0f, -1.0f, 0.0f);

  m_Lights.push_back(Light3);


  Light* Light4 = new Light();

  Light4->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  Light4->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f); // none
  Light4->SetPosition(5.0f, -1.0f, 0.0f);

  m_Lights.push_back(Light4);
}

// -----------------------------------------------------------------------------

bool Lab4m::Frame()
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

bool Lab4m::Render()
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
  m_MultiLightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
    m_PlaneMesh->GetTexture(), m_Lights);
  // Render object (combination of mesh geometry and shader process
  m_MultiLightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
  


  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////