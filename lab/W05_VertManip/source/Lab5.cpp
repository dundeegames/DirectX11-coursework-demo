////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab5.cpp
* \brief      Lab5 class -
*
* \details    Vertex Manipulation
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
#include "Lab5.h"

#include<System/Common.h>
#include<System/MeshGenerator.h>


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab5::Lab5() : m_TMesh(nullptr), m_VertManipShader(nullptr), m_Light(nullptr)
{

}


Lab5::~Lab5()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  DELETE_OBJECT(m_TMesh);
  DELETE_OBJECT(m_VertManipShader);
}


// -----------------------------------------------------------------------------

void Lab5::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  MeshGenerator meshGen = MeshGenerator(m_Direct3D->GetDevice());

  // Create Mesh object
  m_TMesh = meshGen.getPlane(50.0f, 50.0f, 50, 50);
  m_TMesh->setTexture(m_ResourceManager.getTexture(m_Direct3D->GetDevice(), L"brick1.dds"));
  //m_TMesh->setTexture(new Texture(m_Direct3D->GetDevice(), L"../media/bunny.png"));
  m_TMesh->setPosition(0.0f, -5.0f, 25.0f);
  //m_TMesh->setRotation(90.0f, 0.0f, 0.0f);


  m_VertManipShader = new VertManipShader(m_Direct3D->GetDevice(), hwnd);
  m_VertManipShader->InitShader(L"shaders/vertmanip_vs.hlsl", L"shaders/vertmanip_ps.hlsl");

  m_Light = new Light;
  
  m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetDiffuseColour(0.0f, 0.0f, 0.0f, 0.0f);  // none
  //m_Light->SetDiffuseColour(1.0f, 0.5f, 0.5f, 1.0f);  // red
  
  //m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 0.0f); // none
  m_Light->SetAmbientColour(0.25f, 0.25f, 0.25f, 1.0f); // gray
  //m_Light->SetAmbientColour(0.25f, 0.5f, 0.25f, 1.0f);  // green
  m_Light->SetDirection(1.0f, -1.0f, -1.0f); // point facing to

  m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);  // white
  //m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);  // blue
  m_Light->SetSpecularPower(25.0);
}

// -----------------------------------------------------------------------------

bool Lab5::Frame()
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

bool Lab5::Render()
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



  // Get Mesh position
  worldMatrix = m_TMesh->getWorldMatrix();

  // Send geometry data (from mesh)
  m_TMesh->SendData(m_Direct3D->GetDeviceContext());
  // Set shader parameters (matrices and texture)
  m_VertManipShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
                                          m_TMesh->GetTexture(), m_Light, m_Camera, m_Timer);
  // Render object (combination of mesh geometry and shader process
  m_VertManipShader->Render(m_Direct3D->GetDeviceContext(), m_TMesh->GetIndexCount());



  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////