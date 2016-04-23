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
#include <Shaders/Simplex2DheightShader.h>

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

  void updateHeightmap();
  void calculateNormals();

private:
  GeometryShader* m_GeometryShader;
  PointMesh* m_PointMesh;

  UberTessellShader* m_UberShader;
  UberTessellShader* m_UberTessellShader;

  Simplex2DheightShader* m_simplexShader;

  SphereMesh* m_Mesh;
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

  RenderTexture* m_TempMap;
  RenderTexture* m_Heightmap;
  OrthoMesh* m_HeightmapMesh;
  TextureShader* m_NormalShader;
  bool m_mapNotReady;

  OrthoMesh* m_OrthoMesh;
  const XMFLOAT3 m_colourOverlay;

  float tessellationLevel;

};


#endif // PROCEDURAL_H




// 80 //////////////////////////////////////////////////////////////////////////