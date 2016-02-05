////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab3s.h
* \brief      Lab3s class - 
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


#ifndef LAB3S_H
#define LAB3S_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>

#include <Geometry/Deprecated/TriangleMesh.h>
#include <Geometry/Deprecated/QuadMesh.h>
#include <Shaders/TextureShader.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/SpecularShader.h>




// CLASS ///////////////////////////////////////////////////////////////////////

class Lab3s : public BaseApplication
{
public:
  Lab3s();
  ~Lab3s();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  SpecularShader* m_SpecularShader;
  SphereMesh* m_Mesh;
  Light* m_Light;

};


#endif // LAB3S_H




// 80 //////////////////////////////////////////////////////////////////////////