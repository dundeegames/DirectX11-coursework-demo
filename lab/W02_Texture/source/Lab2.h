////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab2.h
* \brief      Lab2 class - Texturing
*
* \details    Texturing 
*
* \author     Jiri Klic
* \version    1.0
* \date       September 2015
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


#ifndef LAB2_H
#define LAB2_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>

#include <Geometry/Deprecated/TriangleMesh.h>
#include <Geometry/Deprecated/QuadMesh.h>
#include <Shaders/TextureShader.h>





// CLASS ///////////////////////////////////////////////////////////////////////

class Lab2 : public BaseApplication
{
public:
  //Lab2(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);
  Lab2();
  ~Lab2();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);
  bool Frame();

private:
  bool Render();

private:
  TextureShader* m_TextureShader;
  //TriangleMesh* m_TMesh;
  QuadMesh* m_TMesh;
  QuadMesh* m_Mesh;


};


#endif // LAB2_H




// 80 //////////////////////////////////////////////////////////////////////////