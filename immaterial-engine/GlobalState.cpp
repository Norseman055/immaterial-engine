#include "DEBUGGING.h"
#include "GlobalState.h"
#include "UserInterface.h"
#include "RenderScene.h"
#include "GraphicsManager.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "ArchiveManager.h"

// animation testing
#include "AnimControllerMan.h"

void createGraphicsWindow() {
	// Set the display mode
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set the window size
	glutInitWindowSize( GAME_WIDTH, GAME_HEIGHT );

	// create the window and name it.
	glutCreateWindow( "GameEngine" );

	// Callback if someone changes size
	glutReshapeFunc( ChangeSize );

	// Callback for key presses
	glutKeyboardFunc( KeyboardKeys );
	glutSpecialFunc( SpecialKeys );

	//call back for Rendering (Draw) Scene
	glutDisplayFunc( RenderScene );

	// Extension library - initialize
	auto err = glewInit();
	if ( GLEW_OK != err ) {
		fprintf( stderr, "GLEW Error: %p\n", glewGetErrorString( err ) );
		// return error
	}

	// add callback timer for game loop
	glutTimerFunc( 16, GameLoopEntry, 1 );

	// setup object
	frameworkStartup();
}

void frameworkStartup() {
	CameraManager::LoadCameras();
	LoadContent();

	SetHumanoidHierarchy();

	auto humanShot = new AnimController( AnimationMan::GetAnimation( "punch" ), static_cast< PyramidObject * >(GraphicsObjMan::GetFirstObj()) );
	humanShot->setPlayback( Loop );
	AnimControllerMan::AddController( humanShot );
};

void LoadContent() {
	// Load archives
	ArchiveMan::LoadArchive( "space_frigate.spu" );
	ArchiveMan::LoadArchive( "warbear_binary.spu" );
	ArchiveMan::LoadArchive( "typhoon.spu" );
	ArchiveMan::LoadArchive( "buggy.spu" );
	ArchiveMan::LoadArchive( "astroboy.spu" );
	ArchiveMan::LoadArchive( "2Bone_Skeleton.spu" );
	ArchiveMan::LoadArchive( "teddy_TGA.spu" );
	ArchiveMan::LoadArchive( "humanoid.spu" );

	// Load models
	ModelManager::LoadModel( "pyramid.cdm" );
	ModelManager::LoadModel( "sphere.cdm" );
	ModelManager::LoadModel( "cube.cdm" );
	ModelManager::LoadModel( "torus.cdm" );

	// Load textures
	TextureManager::AddTexture( "NULL.tga", NOT_INITIALIZED );
	TextureManager::AddTexture( "Rocks.tga", ROCKS );
	TextureManager::AddTexture( "Stone.tga", STONES );
	TextureManager::AddTexture( "RedBrick.tga", RED_BRICK );
	TextureManager::AddTexture( "Duckweed.tga", DUCKWEED );
};

void GameLoop() {
	static Time tCurrent( TIME_ZERO );

	AnimControllerMan::ProcessAnimation( tCurrent );
	AnimControllerMan::SetAnimationPose( tCurrent );
};

void GameLoopEntry( int ) {
	// create a timer, mark begin time.
	static Timer timer;
	auto elapsedTime = timer.toc();

	// mark begin time.
	timer.tic();

	// trigger the game loop, every 16ms
	GameLoop();

	// reset the timer
	glutTimerFunc( 16, GameLoopEntry, 1 );
};