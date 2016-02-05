////////////////////////////////////////////////////////////////////////////////
/**
* \file       RenderStateHelper.h
* \brief      RenderStateHelper class - Saves and Restores render states
*
* \details    Based on RenderStateHelper class from book
*             Real-Time 3D Rendering with DirectX and HLSL by Paul Varcholik
*
* \author     Jiri Klic
* \author     Paul Varcholik
* \version    2.0
* \date       November 2015
* \pre        
* \post       
* \bug        No known bugs
* \warning    
* \todo       
*   
* \copyright  Varcholik, P. 2014.Real-Time 3D Rendering with directX and HLSL.
*             Addison Wesley.
*
* \copyright  University of Abertay - Dundee.2013.
*             Intellectual Property Policy.[online].Available from: 
*             http://www.abertay.ac.uk/media/Intellectual-Property-Policy-v2-01.pdf
*             [Accessed 22 April 2015].
*
*///////////////////////////////////////////////////////////////////////////////


#ifndef RENDERSTATEHELPER_H
#define RENDERSTATEHELPER_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include "d3d.h"





// CLASS ///////////////////////////////////////////////////////////////////////

class RenderStateHelper
{
public:
  RenderStateHelper(ID3D11DeviceContext* deviceContext);
  ~RenderStateHelper();

  static void ResetAll(ID3D11DeviceContext* deviceContext);

  ID3D11RasterizerState* RasterizerState();
  ID3D11BlendState* BlendState();
  ID3D11DepthStencilState* DepthStencilState();

  void SaveRasterizerState();
  void RestoreRasterizerState() const;

  void SaveBlendState();
  void RestoreBlendState() const;

  void SaveDepthStencilState();
  void RestoreDepthStencilState() const;

  void SaveAll();
  void RestoreAll() const;

private:
  RenderStateHelper(const RenderStateHelper& rhs);
  RenderStateHelper& operator=(const RenderStateHelper& rhs);

  ID3D11DeviceContext* m_DeviceContext;

  ID3D11RasterizerState* m_RasterizerState;
  ID3D11BlendState* m_BlendState;
  FLOAT* m_BlendFactor;
  UINT m_SampleMask;
  ID3D11DepthStencilState* m_DepthStencilState;
  UINT m_StencilRef;
};


#endif // RENDERSTATEHELPER_H




// 80 //////////////////////////////////////////////////////////////////////////