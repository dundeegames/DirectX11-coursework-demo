////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab11.cpp
* \brief      Lab11 class - Geometry shader
*
* \details    Geometry shader
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
#include "Lab11.h"
#include <System/Common.h>



// FUNCTIONS ///////////////////////////////////////////////////////////////////


Lab11::Lab11() : m_PointMesh(nullptr), m_GeometryShader(nullptr)
{

}


Lab11::~Lab11()
{
  // Run base application deconstructor
  BaseApplication::~BaseApplication();

  // Release the Direct3D object.
  DELETE_OBJECT(m_PointMesh);
  DELETE_OBJECT(m_GeometryShader);

}


// -----------------------------------------------------------------------------

void Lab11::init(HINSTANCE hinstance, HWND hwnd,
                  int screenWidth, int screenHeight, Input *in)
{
  BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

  // Create Mesh object
  m_PointMesh = new PointMesh(m_Direct3D->GetDevice(), L"../media/DefaultDiffuse.png");

  m_GeometryShader = new GeometryShader(m_Direct3D->GetDevice(), hwnd);

  m_GeometryShader->InitShader(L"shaders/triangle_vs.hlsl",
                               L"shaders/triangle_gs.hlsl",
                               L"shaders/triangle_ps.hlsl");

  

}

// -----------------------------------------------------------------------------

bool Lab11::Frame()
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

bool Lab11::Render()
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
  m_PointMesh->SendData(m_Direct3D->GetDeviceContext());
  
  // Set shader parameters (matrices and texture)
  m_GeometryShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix,
                                        projectionMatrix, m_PointMesh->GetTexture());
  
  // Render object (combination of mesh geometry and shader process
  m_GeometryShader->Render(m_Direct3D->GetDeviceContext(), m_PointMesh->GetIndexCount());
  


  // Present the rendered scene to the screen.
  m_Direct3D->EndScene();

  return true;
}

// -----------------------------------------------------------------------------








// 80 //////////////////////////////////////////////////////////////////////////