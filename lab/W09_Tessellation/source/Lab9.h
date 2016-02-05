////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab9.h
* \brief      Lab9 class - 
*
* \details    Tessellation 
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


#ifndef LAB9_H
#define LAB9_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include <System/BaseApplication.h>


#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/TessellationShader.h>

#include <Geometry/Deprecated/TessellationMesh.h>
#include <System/ResourceManager.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab9 : public BaseApplication
{
public:
  Lab9();
  ~Lab9();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  TessellationShader* m_TessellationShader;
  TessellationMesh* m_Trinagle;

  XMFLOAT2 m_TexturePosition;
  static const XMFLOAT2 m_TextureTranformSpeed;

};


#endif // LAB9_H




// 80 //////////////////////////////////////////////////////////////////////////