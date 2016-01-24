
#include "OpenGL.h"
#include "DEBUGGING.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "GraphicsManager.h"
#include "TextureManager.h"
#include "CameraModel.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "GenericObject.h"
#include "ArchiveManager.h"

// animation testing
#include "PyramidModel.h"
#include "PyramidObject.h"
#include "Anim.h"
#include "AnimControllerMan.h"

void createGraphicsWindow()
{
	// Set the display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set the window size
	glutInitWindowSize(GAME_WIDTH, GAME_HEIGHT);

	// create the window and name it.
	glutCreateWindow("GameEngine");

	// Callback if someone changes size
	glutReshapeFunc(ChangeSize);

	// Callback for key presses
	glutKeyboardFunc(KeyboardKeys);
	glutSpecialFunc(SpecialKeys);

	//call back for Rendering (Draw) Scene
	glutDisplayFunc(RenderScene);
        
	// Extension library - initialize
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		// return error
	}

	// add callback timer for game loop
	glutTimerFunc(16, GameLoopEntry, 1);

	// setup object
	frameworkStartup();

}

void frameworkStartup(void)
{
	CameraMan::LoadCameras();
	LoadContent();

	SetHumanoidHierarchy();

	AnimController *humanShot = new AnimController( AnimationMan::GetAnimation("punch"), (PyramidObject *)GraphicsObjMan::GetFirstObj() );
	humanShot->setPlayback(Loop);
	AnimControllerMan::AddController(humanShot);
};

void LoadContent( void )
{
	// Load archives
	ArchiveMan::LoadArchive("space_frigate.spu");
	ArchiveMan::LoadArchive("warbear_binary.spu");
	ArchiveMan::LoadArchive("typhoon.spu");
	ArchiveMan::LoadArchive("buggy.spu");
	ArchiveMan::LoadArchive("astroboy.spu");
	ArchiveMan::LoadArchive("2Bone_Skeleton.spu");
	ArchiveMan::LoadArchive("teddy_TGA.spu");
	ArchiveMan::LoadArchive("humanoid.spu");

	// Load models
	ModelMan::LoadModel("pyramid.cdm");
	ModelMan::LoadModel("sphere.cdm");
	ModelMan::LoadModel("cube.cdm");
	ModelMan::LoadModel("torus.cdm");

	// Load textures
	TextureMan::AddTexture( "NULL.tga", NOT_INITIALIZED );
	TextureMan::AddTexture( "Rocks.tga", ROCKS );
	TextureMan::AddTexture( "Stone.tga", STONES );
	TextureMan::AddTexture( "RedBrick.tga", RED_BRICK );
	TextureMan::AddTexture( "Duckweed.tga", DUCKWEED );
};

void GameLoop( void )
{
	// This function is called once every frame.
	// Use this function to control process order
	// Input, AI, Physics, Animation, and Graphics

	static Time tCurrent(TIME_ZERO);

	AnimControllerMan::ProcessAnimation( tCurrent );
	AnimControllerMan::SetAnimationPose( tCurrent );

};

void GameLoopEntry(int )
{
	// create a timer, mark begin time.
	static Timer timer;
	Time elapsedTime = timer.toc();

	// mark begin time.
	timer.tic();

	// trigger the game loop, every 16ms
	GameLoop();

	// reset the timer
	glutTimerFunc(16, GameLoopEntry, 1);
};