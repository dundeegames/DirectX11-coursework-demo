////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab10.h
* \brief      Lab10 class -
*
* \details    Shadow maps
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
#include "Lab10.h"
#include <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab10::Lab10() : m_Trinagle(nullptr), m_ShadowShader(nullptr)
{

}


Lab10::~Lab10()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_Trinagle);
  DELETE_OBJECT(m_ShadowShader);

}


// -----------------------------------------------------------------------------

void Lab10::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create Mesh object
  m_Trinagle = new TessellationMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);
  m_ShadowShader->InitShader(L"shaders/shadow_vs.hlsl", L"shaders/shadow_ps.hlsl");
  
}

// -----------------------------------------------------------------------------

bool Lab10::Frame()
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

bool Lab10::Render()
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


  // Send geometry data (from mesh)
  m_Trinagle->SendData(m_Direct3D->GetDeviceContext());
  
  // Set shader parameters (matrices and texture)
  m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                            projectionMatrix, m_Trinagle->GetTexture());

  // Render object (combination of mesh geometry and shader process
  m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_Trinagle->GetIndexCount());
  


  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////