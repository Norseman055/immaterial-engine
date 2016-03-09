// ===== MY HEADERS =====
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "GlobalState.h"
#include "RenderState.h"
#include "PyramidModel.h"

// ----------------------------------------------------------------------------
// Score card - How many do we have?
// Goal remove them all into proper systems
// Keep all Globals defined here, extern elsewhere
// ----------------------------------------------------------------------------

PyramidModel		*myPyramid;

// ----------------------------------------------------------------------------
// Uncaught exception handler to help program exit cleanly
// ----------------------------------------------------------------------------
void terminate_prog() {
	out( "Main(): ---------- TERMINATED ----------" );
	exit( -1 );
}

// ----------------------------------------------------------------------------
// Main entry point for GLUT based programs
int main( int argc, char* argv[] ) {
	try {
		// set directory
		gltSetWorkingDirectory( argv[0] );

		// initialize GLUT
		glutInit( &argc, argv );

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
	} catch ( ... ) {
		terminate_prog();
	}
}