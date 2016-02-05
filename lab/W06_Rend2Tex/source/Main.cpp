// Main.cpp
#include <System/System.h>
#include "Lab6.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  // Create the system object with Lab5
  System* m_System = new System(new Lab6());

  // Initialize and run the system object.
  m_System->Run();

  // Shutdown and release the system object.
  delete m_System;
  m_System = 0;

  return 0;
}