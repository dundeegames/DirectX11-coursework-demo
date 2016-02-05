// Application.h
#ifndef _LAB1_H
#define _LAB1_H

// Includes
#include <System/BaseApplication.h>
#include <Geometry/Deprecated/TriangleMesh.h>
#include <Geometry/Deprecated/QuadMesh.h>
#include <Shaders/ColourShader.h>

class Lab1 : public BaseApplication
{
public:

	/*Lab1(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);*/
  Lab1();
  ~Lab1();

  void init(HINSTANCE hinstance, HWND hwnd,
              int screenWidth, int screenHeight, Input *in);
	bool Frame();

private:
	bool Render();

private:
	ColourShader* m_ColourShader;
	//TriangleMesh* m_Mesh;
  QuadMesh* m_Mesh;
};

#endif