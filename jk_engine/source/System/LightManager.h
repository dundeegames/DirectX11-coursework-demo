////////////////////////////////////////////////////////////////////////////////
/**
* \file       LightManager.h
* \brief      LightManager class - 
*
* \details    Detailed description 
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


#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <d3d11.h>
#include "Light.h"



#define MAX_LIGHTS 4

// CLASS ///////////////////////////////////////////////////////////////////////

class LightManager
{
private:
  struct LightSpec
  {
    XMFLOAT4 ambientColour;
    XMFLOAT4 diffuseColour;
    XMFLOAT4 specularColour;
    XMFLOAT4 position;          //XMFLOAT4.w = 0 (directional light) or 1 (point light)
    XMFLOAT4 attenuation;       //x=Constant, y=Linear, z=Quadratic, w=Range
    XMFLOAT4 spotDirection;     // spotlight use .xyz
    
    float m_specularPower;
    float Phi;                  // Outher cone
    float Theta;                // Inner cone
    bool isActive;
  };


  struct LightPool
  {
    LightSpec light[MAX_LIGHTS];
  };


public:
  LightManager();
  ~LightManager();

  void updateLights(ID3D11Buffer* lightBuffer);


private:
  LightPool m_LightPool;


};


#endif // LIGHTMANAGER_H




// 80 //////////////////////////////////////////////////////////////////////////