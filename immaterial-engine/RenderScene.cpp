
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "MathEngine.h"
#include "PyramidModel.h"
#include "GraphicsManager.h"
#include "CameraManager.h"
#include "CameraObject.h"

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{    
	out("RenderScene: -----------------\n");

	// Clear the window with current clearing color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Our new camera, make sure everything is consistent
	CameraObject *cam = CameraMan::GetCurrCamera();
	cam->updateCamera();

	// Process & Draw the objects
	GraphicsObjMan::drawObjects();

	// Redraw
	glutPostRedisplay();

	// Flush drawing commands
	glutSwapBuffers();
}