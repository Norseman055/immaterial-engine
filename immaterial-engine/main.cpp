
// ===== MY HEADERS =====
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "GlobalState.h"
#include "RenderState.h"
#include "CameraModel.h"
#include "Anim.h"
#include "PyramidModel.h"

// ----------------------------------------------------------------------------
// Score card - How many do we have?  
// Goal remove them all into proper systems
// Keep all Globals defined here, extern elsewhere
// ----------------------------------------------------------------------------

GLShaderManager		shaderManager;
PyramidModel		*myPyramid;

//-----------------------------------------------------------------------------
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	// checking for mem leaks
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// set directory
	gltSetWorkingDirectory(argv[0]);
	
	// initialize GLUT
	glutInit(&argc, argv);

	// create graphics windows with defaults
	createGraphicsWindow();

	// Set up the Render Context
	// Internal context (state) for the graphics chips
	SetupRC();

	// Main Loop, stays running 
	// Processes, messages, keystrokes until window is closed
	glutMainLoop();
    
	// Closes the rendering context before quitting, removes textures
	ShutdownRC();

	return 0;
}