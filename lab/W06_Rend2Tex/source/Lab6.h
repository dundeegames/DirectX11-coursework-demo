////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab6.h
* \brief      Lab6 class - 
*
* \details    Render to Texture
*
* \author     Jiri Klic
* \version    1.0
* \date       October 2015
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


#ifndef LAB6_H
#define LAB6_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/ResourceManager.h>
#include <System/RenderTexture.h>

#include <Shaders/VertManipShader.h>
#include <Shaders/TextureShader.h>

#include <Geometry/Mesh.h>
#include <Geometry/Deprecated/OrthoMesh.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab6 : public BaseApplication
{
public:
  Lab6();
  ~Lab6();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();
  void RenderToTexture();
  void RenderScene();

private:
  VertManipShader* m_VertManipShader;
  TextureShader* m_TextureShader;
  Mesh* m_TMesh;
  Light* m_Light;
  ResourceManager m_ResourceManager;

  RenderTexture* m_RenderTexture;
  OrthoMesh* m_OrthoMesh;

};


#endif // LAB6_H




// 80 //////////////////////////////////////////////////////////////////////////