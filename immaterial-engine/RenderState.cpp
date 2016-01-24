
// MY HEADERS
#include "OpenGL.h"
#include "DEBUGGING.h"
#include "RenderState.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "AnimControllerMan.h"
#include "GraphicsManager.h"
#include "ModelManager.h"

// GL GLOBALS
extern GLShaderManager shaderManager;

// ----------------------------------------------------------------------------
// Cleanup... such as deleting texture objects
void ShutdownRC(void)
{
	AnimControllerMan::DeleteControllers();
	AnimationMan::DeleteAnimations();
	ModelMan::DeleteModels();
	TextureMan::DeleteTextures();
}

// ------------------------------------------------------------------------
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
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
    
