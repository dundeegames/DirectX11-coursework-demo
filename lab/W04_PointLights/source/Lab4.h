////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab4.h
* \brief      Lab4 class - 
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


#ifndef LAB4_H
#define LAB4_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>

#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/PointLightShader.h>

#include <Geometry/Mesh.h>
#include <System/ResourceManager.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab4 : public BaseApplication
{
public:
  Lab4();
  ~Lab4();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  PointLightShader* m_PointLightShader;
  SphereMesh* m_Mesh;
  Mesh* m_PlaneMesh;
  Light* m_Light;
  ResourceManager m_ResourceManager;
};


#endif // LAB4_H




// 80 //////////////////////////////////////////////////////////////////////////