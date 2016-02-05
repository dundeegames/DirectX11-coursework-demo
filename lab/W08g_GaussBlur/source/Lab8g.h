////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab8g.h
* \brief      Lab8g class - 
*
* \details    Post Processing - Gaussian Blur
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


#ifndef LAB8G_H
#define LAB8G_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/RenderTexture.h>

#include <Shaders/SpecularShader.h>
#include <Shaders/HorizontalBlurShader.h>
#include <Shaders/VerticalBlurShader.h>
#include <Shaders/TextureShader.h>

#include <Geometry/Mesh.h>
#include <Geometry/Deprecated/CubeMesh.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <Geometry/Deprecated/OrthoMesh.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab8g : public BaseApplication
{
public:
  Lab8g();
  ~Lab8g();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

  void RenderToTexture();
  void DownsampleTexture();
  void ApplyHorBlur();
  void ApplyVerBlur();
  void UpsampleTexture();
  void RenderScene();

  void DrawGeometry(Camera* camera);

private:
  float m_ScrWidth;
  float m_ScrHeight;

  SpecularShader* m_SpecularShader;
  HorizontalBlurShader* m_HorizontalBlurShader;
  VerticalBlurShader* m_VerticalBlurShader;
  TextureShader* m_TextureShader;

  Camera* m_AnotherCamera;
  SphereMesh* m_Mesh;
  CubeMesh* m_Cube;
  Light* m_Light;


  RenderTexture* m_RenderTexture1;
  RenderTexture* m_RenderTexture2;
  OrthoMesh* m_OrthoMesh;

};


#endif // LAB8G_H




// 80 //////////////////////////////////////////////////////////////////////////