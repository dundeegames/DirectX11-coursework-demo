////////////////////////////////////////////////////////////////////////////////
/**
* \file       Texture.h
* \brief      Texture class -
*
* \details    
*
* \author     Paul Robertson
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


#ifndef TEXTURE_H
#define TEXTURE_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <string>
#include <fstream>

using namespace DirectX;


// CLASS ///////////////////////////////////////////////////////////////////////

class Texture
{
public:
  Texture(ID3D11Device* device, const WCHAR* filename);
  ~Texture();

  ID3D11ShaderResourceView* GetTexture();

private:
  bool does_file_exist(const WCHAR *fileName);

  ID3D11ShaderResourceView* m_texture;

};


#endif // TEXTURE_H




// 80 //////////////////////////////////////////////////////////////////////////