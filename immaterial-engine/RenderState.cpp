
// MY HEADERS
#include "DEBUGGING.h"
#include "RenderState.h"
#include "AnimControllerMan.h"
#include "GraphicsManager.h"
#include "ModelManager.h"
#include "CameraManager.h"

// GL GLOBALS
extern GLShaderManager shaderManager;

// ----------------------------------------------------------------------------
// Cleanup... such as deleting texture objects
void ShutdownRC(void) {
	GraphicsObjMan::DeleteGraphicsObjects();
	AnimControllerMan::DeleteControllers();
	AnimationMan::DeleteAnimations();
	TextureMan::DeleteTextures();
	ModelMan::DeleteModels();
	CameraMan::DeleteCameras();
}

// ------------------------------------------------------------------------
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC() {
    // Black background
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f );

	// Initialize Shaders
	shaderManager.InitializeStockShaders();

	// culling enable
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	// Turn on Depth Tests
	glEnable(GL_DEPTH_TEST);
}
    
