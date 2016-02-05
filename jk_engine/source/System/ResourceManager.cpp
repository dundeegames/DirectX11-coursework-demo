////////////////////////////////////////////////////////////////////////////////
/**
* \file       ResourceManager.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include"ResourceManager.h"
#include"Common.h"



// FUNCTIONS ///////////////////////////////////////////////////////////////////


ResourceManager::ResourceManager()
{
  images_path = L"../media/";
}


ResourceManager::~ResourceManager()
{
  for (auto it = texture_list.begin(); it != texture_list.end(); it++)
  {
    DELETE_OBJECT(it->second);
  }
}


// -----------------------------------------------------------------------------

Texture* ResourceManager::getTexture(ID3D11Device* device, const WCHAR* file)
{
  if (texture_list.find(file) == texture_list.end())
  {
    loadTexture(device, file);
  }

  return texture_list[file];
}

// -----------------------------------------------------------------------------

void ResourceManager::loadTexture(ID3D11Device* device, const WCHAR* file)
{
  std::wstring path;

  path = images_path;
  path += file;

  texture_list[file] = new Texture(device, path.c_str());

}

// -----------------------------------------------------------------------------












// 80 //////////////////////////////////////////////////////////////////////////