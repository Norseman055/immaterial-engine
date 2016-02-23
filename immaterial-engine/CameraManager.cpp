#include "CameraManager.h"

#include "GraphicsManager.h"
#include "DEBUGGING.h"

void CameraManager::LoadCameras() {
	// Create camera0 (watches objects)
	auto cam0 = new Camera( CAMERA_CULLING );

	cam0->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam0->setPerspective( 35.0f, float( GAME_WIDTH ) / float( GAME_HEIGHT ), 1.0f, 10000.0f );
	cam0->setOrientAndPosition( Vect( 0.0f, 1.0f, 0.0f ), Vect( 0.0f, 0.0f, 10.0f ), Vect( 180.0f, 0.0f, 2000.0f ) );
	cam0->updateCamera();

	Add( new CameraNode( cam0 ) );
	GraphicsObjMan::AddDebugObject( cam0 );

	// create overview camera (watches camera0)
	auto cam1 = new Camera( CAMERA_OVERVIEW );

	cam1->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam1->setPerspective( 35.0f, float( GAME_WIDTH ) / float( GAME_HEIGHT ), 1.0f, 10000.0f );
	cam1->setOrientAndPosition( Vect( 0.0f, 0.0f, 1.0f ), Vect( 0.0f, 0.0f, 4.0f ), Vect( 500.0f, 0.0f, 4.0f ) );

	Add( new CameraNode( cam1 ) );

	SetCurrentCamera( cam0 );

	// create the camera model
	auto myCameraModel = new CameraModel;
	myCameraModel->createVAO( cam0 );

	cam0->setCameraModel( myCameraModel );
}

void CameraManager::RemoveCurrentCamera() {
	// remove current camera from the list
	auto walker = static_cast< CameraNode * >(GetObjectList()->getRoot()->getChild());
	auto tmp = GetNextCamera();

	// do not remove the culling camera. i want that one at all times.
	auto currCamera = GetCurrentCamera();
	if ( currCamera->getName() != CAMERA_CULLING ) {
		while ( walker != nullptr ) {
			if ( walker->getData() == currCamera ) {
				Remove( walker );
				SetCurrentCamera( tmp );
			}
			walker = static_cast< CameraNode * >(walker->getSibling());
		}
	}
}

void CameraManager::SetCurrentCamera( Camera* const inCam ) {
	privGetInstance()->currentCamera = inCam;
}

void CameraManager::SwitchState() {
	auto cMan = privGetInstance();

	if ( cMan->cameraState == ORBIT ) {
		cMan->cameraState = ROTATE;
	} else {
		cMan->cameraState = ORBIT;
	}

	out( "Camera view state changed.\n" );
}

Camera* CameraManager::Find( const CameraName inName ) {
	// find the camera node
	auto walker = static_cast< CameraNode * >(GetObjectList()->getRoot()->getChild());

	while ( walker != nullptr ) {
		if ( walker->getData()->getName() == inName ) {
			break;
		}
		walker = static_cast< CameraNode * >(walker->getSibling());
	}

	if ( walker == nullptr ) {
		walker = static_cast< CameraNode * >(GetObjectList()->getRoot()->getChild());
	}

	return walker->getData();
}

Camera* CameraManager::GetCurrentCamera() {
	return privGetInstance()->currentCamera;
}

Camera* CameraManager::GetNextCamera() {
	auto cMan = privGetInstance();

	// return next camera in list
	auto walker = reinterpret_cast< CameraNode* >(cMan->currentCamera);

	if ( walker->getSibling() != nullptr ) {
		walker = static_cast< CameraNode* >(walker->getSibling());
	} else {
		walker = static_cast< CameraNode* >(GetObjectList()->getRoot()->getChild());
	}

	return walker->getData();
}

CameraState CameraManager::GetState() {
	return privGetInstance()->cameraState;
}

CameraManager* CameraManager::privGetInstance() {
	static CameraManager instance;
	return &instance;
}

CameraManager::CameraManager()
	: currentCamera( nullptr ), cameraState( ORBIT ) { }