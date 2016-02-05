// Main.cpp
#include <System/System.h>
#include "Lab6s.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  // Create the system object.
  System* m_System = new System(new Lab6s());
	
	// Initialize and run the system object.
	m_System->Run();
	
	// Shutdown and release the system object.
	delete m_System;
	m_System = nullptr;

	return 0;
}