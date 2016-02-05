////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab3.h
* \brief      Lab3 class - 
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


#ifndef LAB3_H
#define LAB3_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>

#include <Geometry/Deprecated/TriangleMesh.h>
#include <Geometry/Deprecated/QuadMesh.h>
#include <Shaders/TextureShader.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/LightShader.h>




// CLASS ///////////////////////////////////////////////////////////////////////

class Lab3 : public BaseApplication
{
public:
  //Lab3(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);
  Lab3();
  ~Lab3();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  LightShader* m_LightShader;
  SphereMesh* m_Mesh;
  Light* m_Light;

};


#endif // LAB3_H




// 80 //////////////////////////////////////////////////////////////////////////