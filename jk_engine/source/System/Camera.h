////////////////////////////////////////////////////////////////////////////////
/**
* \file       Camera.h
* \brief      Camera class - 
*
* \details    extended lab camera 
*
* \author     Jiri Klic
* \version    2.0
* \date       December 2015
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


#ifndef CAMERA_H
#define CAMERA_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <directxmath.h>

using namespace DirectX;





// CLASS ///////////////////////////////////////////////////////////////////////

class Camera
{
public:
  void* operator new(size_t i)
  {
    return _mm_malloc(i, 16);
  }

    void operator delete(void* p)
  {
    _mm_free(p);
  }

  Camera();
  Camera(const Camera&);
  ~Camera();

  void SetPosition(float, float, float);
  void SetRotation(float, float, float);

  XMFLOAT3 GetPosition();
  XMFLOAT4 GetUp() { return m_up; };
  XMVECTOR GetRotation();

  void Update();
  void GetViewMatrix(XMMATRIX&);
  void GetBaseViewMatrix(XMMATRIX&);

  void SetFrameTime(float);

  void MoveForward();
  void MoveBackward();
  void MoveUpward();
  void MoveDownward();
  void TurnLeft();
  void TurnRight();
  void TurnUp();
  void TurnDown();
  void StrafeRight();
  void StrafeLeft();

private:
  float m_positionX, m_positionY, m_positionZ;
  float m_rotationX, m_rotationY, m_rotationZ;
  XMMATRIX m_viewMatrix;
  XMFLOAT4 m_up;
  float m_speed, m_frameTime;;
};


#endif // CAMERA_H




// 80 //////////////////////////////////////////////////////////////////////////