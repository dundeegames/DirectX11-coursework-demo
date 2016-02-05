////////////////////////////////////////////////////////////////////////////////
/**
* \file       RenderStateHelper.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "RenderStateHelper.h"
#include "Common.h"



// FUNCTIONS ///////////////////////////////////////////////////////////////////


RenderStateHelper::RenderStateHelper(ID3D11DeviceContext* deviceContext) :
                         m_DeviceContext(deviceContext), m_RasterizerState(nullptr),
                         m_BlendState(nullptr), m_DepthStencilState(nullptr), 
                         m_SampleMask(UINT_MAX), m_StencilRef(UINT_MAX),
                         m_BlendFactor(new FLOAT[4])
{

}

RenderStateHelper::~RenderStateHelper()
{
  RELEASE_OBJECT(m_RasterizerState);
  RELEASE_OBJECT(m_BlendState);
  RELEASE_OBJECT(m_DepthStencilState);
  DELETE_OBJECTS(m_BlendFactor);
}


// -----------------------------------------------------------------------------

void RenderStateHelper::ResetAll(ID3D11DeviceContext* deviceContext)
{
  deviceContext->RSSetState(nullptr);
  deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
  deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
}

// -----------------------------------------------------------------------------

ID3D11RasterizerState* RenderStateHelper::RasterizerState()
{
  return m_RasterizerState;
}

// -----------------------------------------------------------------------------

ID3D11BlendState* RenderStateHelper::BlendState()
{
  return m_BlendState;
}

// -----------------------------------------------------------------------------

ID3D11DepthStencilState* RenderStateHelper::DepthStencilState()
{
  return m_DepthStencilState;
}

// -----------------------------------------------------------------------------

void RenderStateHelper::SaveRasterizerState()
{
  RELEASE_OBJECT(m_RasterizerState);
  m_DeviceContext->RSGetState(&m_RasterizerState);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::RestoreRasterizerState() const
{
  m_DeviceContext->RSSetState(m_RasterizerState);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::SaveBlendState()
{
  RELEASE_OBJECT(m_BlendState);
  m_DeviceContext->OMGetBlendState(&m_BlendState, m_BlendFactor, &m_SampleMask);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::RestoreBlendState() const
{
  m_DeviceContext->OMSetBlendState(m_BlendState, m_BlendFactor, m_SampleMask);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::SaveDepthStencilState()
{
  RELEASE_OBJECT(m_DepthStencilState);
  m_DeviceContext->OMGetDepthStencilState(&m_DepthStencilState, &m_StencilRef);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::RestoreDepthStencilState() const
{
  m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, m_StencilRef);
}

// -----------------------------------------------------------------------------

void RenderStateHelper::SaveAll()
{
  SaveRasterizerState();
  SaveBlendState();
  SaveDepthStencilState();
}

// -----------------------------------------------------------------------------

void RenderStateHelper::RestoreAll() const
{
  RestoreRasterizerState();
  RestoreBlendState();
  RestoreDepthStencilState();
}

// -----------------------------------------------------------------------------







// 80 //////////////////////////////////////////////////////////////////////////