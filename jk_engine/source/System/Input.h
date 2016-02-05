////////////////////////////////////////////////////////////////////////////////
/**
* \file       Input.h
* \brief      Input class - 
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


#ifndef INPUT_H
#define INPUT_H


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <Windows.h>





// CLASS ///////////////////////////////////////////////////////////////////////

class Input
{
  typedef struct Mouse
  {
    int x, y;
    bool left, right;
  };

public:
  Input();
  ~Input();

  void SetKeyDown(WPARAM key);
  void SetKeyUp(WPARAM key);

  bool isKeyDown(int key);
  void setMouseX(int xPosition);
  void setMouseY(int yPosition);
  int getMouseX();
  int getMouseY();
  void setLeftMouse(bool down);
  void setRightMouse(bool down);
  bool isLeftMouseDown();
  bool isRightMouseDown();

private:
  bool keys[256];
  Mouse mouse;

  void initValues();

};


#endif // INPUT_H




// 80 //////////////////////////////////////////////////////////////////////////