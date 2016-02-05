////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab11.h
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


#ifndef LAB11_H
#define LAB11_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/ResourceManager.h>

#include <Geometry/Deprecated/PointMesh.h>

#include <Shaders/GeometryShader.h>



// CLASS ///////////////////////////////////////////////////////////////////////

class Lab11 : public BaseApplication
{
public:
  Lab11();
  ~Lab11();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  GeometryShader* m_GeometryShader;
  PointMesh* m_PointMesh;

};


#endif // LAB11_H




// 80 //////////////////////////////////////////////////////////////////////////