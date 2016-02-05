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


#ifndef LAB10_H
#define LAB10_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/ResourceManager.h>

#include <Shaders/ShadowShader.h>

#include <Geometry/Deprecated/TessellationMesh.h>



// CLASS ///////////////////////////////////////////////////////////////////////

class Lab10 : public BaseApplication
{
public:
  Lab10();
  ~Lab10();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  ShadowShader* m_ShadowShader;
  TessellationMesh* m_Trinagle;

};


#endif // LAB10_H




// 80 //////////////////////////////////////////////////////////////////////////