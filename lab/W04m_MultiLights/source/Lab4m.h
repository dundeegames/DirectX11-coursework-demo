////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab4m.h
* \brief      Lab4m class - 
*
* \details    Multiple Lights 
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


#ifndef LAB4M_H
#define LAB4M_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include <System/BaseApplication.h>


#include <Geometry/Deprecated/SphereMesh.h>
#include <System/Light.h>
#include <Shaders/MultiLightShader.h>

#include <Geometry/Mesh.h>
#include <System/ResourceManager.h>


// CLASS ///////////////////////////////////////////////////////////////////////

class Lab4m : public BaseApplication
{
public:
  Lab4m();
  ~Lab4m();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  MultiLightShader* m_MultiLightShader;
  SphereMesh* m_Mesh;
  Mesh* m_PlaneMesh;

  std::vector<Light*> m_Lights;
  ResourceManager m_ResourceManager;
};


#endif // LAB4M_H




// 80 //////////////////////////////////////////////////////////////////////////