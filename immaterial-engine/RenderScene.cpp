
#include "DEBUGGING.h"
#include "PyramidModel.h"
#include "GraphicsManager.h"
#include "CameraManager.h"

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene() {    
	out("RenderScene: -----------------\n");

	// Clear the window with current clearing color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Our new camera, make sure everything is consistent
	CameraMan::GetCurrCamera()->updateCamera();

	// Process & Draw the objects
	GraphicsObjMan::DrawObjects();

	// Redraw
	glutPostRedisplay();

	// Flush drawing commands
	glutSwapBuffers();
}