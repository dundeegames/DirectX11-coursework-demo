// System.h
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

// INCLUDES 
#include <windows.h>
//#include "Lab1.h"
//#include "Lab2.h"
//#include "Lab3.h"
#include <System/BaseApplication.h>
#include "Input.h"

class System
{
public:
	System(BaseApplication* application);
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	
  BaseApplication* m_Application;
	Input m_Input;

//Prototypes
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

//Globals
static System* ApplicationHandle = 0;

#endif