////////////////////////////////////////////////////////////////////////////////
/**
* \file       ResourceManager.h
* \brief      ResourceManager class - 
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


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include<d3d11.h>
#include<Geometry/Texture.h>
#include<map>
#include<string>



// CLASS ///////////////////////////////////////////////////////////////////////

class ResourceManager
{

public:
  ResourceManager();
  ~ResourceManager();

  Texture* getTexture(ID3D11Device* device, const WCHAR* file);


private:
  void loadTexture(ID3D11Device* device, const WCHAR* file);

  std::wstring images_path;
  std::map<const WCHAR*, Texture*> texture_list;

};


#endif // RESOURCE_MANAGER_H




// 80 //////////////////////////////////////////////////////////////////////////