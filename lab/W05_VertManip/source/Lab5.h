////////////////////////////////////////////////////////////////////////////////
/**
* \file       Lab5.h
* \brief      Lab3s class - 
*
* \details    Vertex Manipulation
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


#ifndef LAB5_H
#define LAB5_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <System/BaseApplication.h>
#include <System/Light.h>
#include <Shaders/VertManipShader.h>

#include <Shaders/LightShader.h>

#include <Geometry/Mesh.h>
#include <System/ResourceManager.h>

// CLASS ///////////////////////////////////////////////////////////////////////

class Lab5 : public BaseApplication
{
public:
  Lab5();
  ~Lab5();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  bool Render();

private:
  VertManipShader* m_VertManipShader;
  Mesh* m_TMesh;
  Light* m_Light;
  ResourceManager m_ResourceManager;

};


#endif // LAB5_H




// 80 //////////////////////////////////////////////////////////////////////////