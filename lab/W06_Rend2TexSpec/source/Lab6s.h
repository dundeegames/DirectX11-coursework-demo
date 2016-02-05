////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab6s.h
* \brief      Lab6s class - 
*
* \details    Render to Texture, based on Lab3 Specular 
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


#ifndef LAB6S_H
#define LAB6S_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>

#include <Shaders/TextureShader.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/SpecularShader.h>

#include <System/RenderTexture.h>
#include <Shaders/TextureShader.h>
#include <Geometry/Deprecated/OrthoMesh.h>

// CLASS ///////////////////////////////////////////////////////////////////////

class Lab6s : public BaseApplication
{
public:
  Lab6s();
  ~Lab6s();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();
  void RenderToTexture();
  void RenderScene();

private:
  SpecularShader* m_Shader;
  SphereMesh* m_Mesh;
  Light* m_Light;
  Camera* m_AnotherCamera;
  
  TextureShader* m_TextureShader;
  RenderTexture* m_RenderTexture;
  OrthoMesh* m_OrthoMesh;

};


#endif // LAB3S_H




// 80 //////////////////////////////////////////////////////////////////////////