////////////////////////////////////////////////////////////////////////////////
/**
* \file       Input.cpp
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





// INCLUDES ////////////////////////////////////////////////////////////////////
#include "Input.h"




// FUNCTIONS ///////////////////////////////////////////////////////////////////


Input::Input()
{
  initValues();
}


Input::~Input()
{

}


// -----------------------------------------------------------------------------

void Input::initValues()
{
  for (int i = 0; i < 256; i++)
  {
    keys[i] = false;
  }

}


// -----------------------------------------------------------------------------

void Input::SetKeyDown(WPARAM key)
{
  keys[key] = true;
}

// -----------------------------------------------------------------------------

void Input::SetKeyUp(WPARAM key)
{
  keys[key] = false;
}

// -----------------------------------------------------------------------------

bool Input::isKeyDown(int key)
{
  return keys[key];
}

// -----------------------------------------------------------------------------

void Input::setMouseX(int xPosition)
{
  mouse.x = xPosition;
}

// -----------------------------------------------------------------------------

void Input::setMouseY(int yPosition)
{
  mouse.y = yPosition;
}

// -----------------------------------------------------------------------------

int Input::getMouseX()
{
  return mouse.x;
}

// -----------------------------------------------------------------------------

int Input::getMouseY()
{
  return mouse.y;
}

// -----------------------------------------------------------------------------

void Input::setLeftMouse(bool down)
{
  mouse.left = down;
}

// -----------------------------------------------------------------------------

void Input::setRightMouse(bool down)
{
  mouse.right = down;
}

// -----------------------------------------------------------------------------

bool Input::isLeftMouseDown()
{
  return mouse.left;
}

// -----------------------------------------------------------------------------

bool Input::isRightMouseDown()
{
  return mouse.right;
}

// -----------------------------------------------------------------------------






// 80 //////////////////////////////////////////////////////////////////////////

