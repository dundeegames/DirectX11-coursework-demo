////////////////////////////////////////////////////////////////////////////////
/**
* \file       Procedural.h
* \brief      Procedural class
*
* \details     
*
* \author     Jiri Klic
* \version    1.0
* \date       February 2016
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


#ifndef PROCEDURAL_H
#define PROCEDURAL_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <vector>

#include <SpriteBatch.h>
#include <SpriteFont.h>


#include <System/BaseApplication.h>
#include <System/Light.h>
#include <System/ResourceManager.h>
#include <System/RenderStateHelper.h>
#include <System/RenderTexture.h>

#include <Shaders/GeometryShader.h>
#include <Shaders/TessellationShader.h>
#include <Shaders/UberShader.h>
#include <Shaders/UberTessellShader.h>
#include <Shaders/TextureShader.h>

#include <Geometry/Deprecated/PointMesh.h>
#include <Geometry/Deprecated/CubeMesh.h>
#include <Geometry/Deprecated/SphereMesh.h>
#include <Geometry/Deprecated/OrthoMesh.h>

#include <Geometry/Mesh.h>
#include <Geometry/TerrainMesh.h>

// CLASS ///////////////////////////////////////////////////////////////////////

enum RenderStage { DISPLACEMENT_OFF_STAGE, DISPLACEMENT_ON_STAGE };
enum EffectStage { NORMAL_STAGE, GRAYSCALE_STAGE, INVERSE_STAGE, COLOURED_STAGE };



class Procedural : public BaseApplication
{
public:
  Procedural();
  ~Procedural();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);

  bool Frame();

private:
  void initLights();
  bool Render();
  void renderToTexture();
  void renderOrthoMesh();
  void drawGeometry();

private:
  GeometryShader* m_GeometryShader;
  PointMesh* m_PointMesh;

  UberShader* m_UberShader;
  TessellationShader* m_TessellationShader;
  UberTessellShader* m_UberTessellShader;


  SphereMesh* m_Mesh;
  CubeMesh* m_Cube;
  TerrainMesh* m_Terrain;
  Mesh* m_PlaneMesh;

  Light* m_Lights[MAX_LIGHTS];

  RenderStateHelper* m_RendStateHelp;

  SpriteBatch* m_SpriteBatch;
  SpriteFont* m_SpriteFont;
  XMFLOAT2 m_TextPosition;

  RenderStage m_RenderStage;
  EffectStage m_EffectStage;

  TextureShader* m_TextureShader;
  RenderTexture* m_RenderTexture;
  OrthoMesh* m_OrthoMesh;
  const XMFLOAT3 m_colourOverlay;

};


#endif // PROCEDURAL_H




// 80 //////////////////////////////////////////////////////////////////////////