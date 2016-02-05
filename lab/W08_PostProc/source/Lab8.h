////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab8.h
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


#ifndef LAB8_H
#define LAB8_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/RenderTexture.h>

#include <Shaders/SpecularShader.h>
#include <Shaders/BoxBlurShader.h>
#include <Shaders/TextureShader.h>

#include <Geometry/Mesh.h>
#include <Geometry/Deprecated/CubeMesh.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <Geometry/Deprecated/OrthoMesh.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab8 : public BaseApplication
{
public:
  Lab8();
  ~Lab8();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

  void RenderToTexture();
  void ApplyBoxBlur();
  void RenderScene();
  void DrawGeometry(Camera* camera);

private:
  ScreenSizeBufferType m_scrSizeBuf;

  SpecularShader* m_SpecularShader;
  BoxBlurShader* m_BoxBlurShader;
  TextureShader* m_TextureShader;

  Camera* m_AnotherCamera;
  SphereMesh* m_Mesh;
  CubeMesh* m_Cube;
  Light* m_Light;


  RenderTexture* m_RenderTexture1;
  RenderTexture* m_RenderTexture2;
  OrthoMesh* m_OrthoMesh;

};


#endif // LAB8_H




// 80 //////////////////////////////////////////////////////////////////////////