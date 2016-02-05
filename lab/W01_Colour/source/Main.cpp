// Main.cpp
#include <System/System.h>

#include "Lab1.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  Lab1* lab = new Lab1();
	System* m_System;
		
	// Create the system object.
	m_System = new System(lab);
	
	// Initialize and run the system object.
	m_System->Run();
	
	// Shutdown and release the system object.
	delete m_System;
	m_System = 0;

	return 0;
}