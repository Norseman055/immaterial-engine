#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CameraManager.h"
#include "GraphicsManager.h"

CameraMan::CameraMan()
	: active( nullptr ), currCamera( nullptr ), camState( ORBIT ) { }

// singleton
CameraMan* CameraMan::privGetInstance() {
	static CameraMan cameraMan;
	return &cameraMan;
}

void CameraMan::LoadCameras() {
	// Create camera0 (watches objects)
	auto cam0 = new CameraObject( CAMERA_CULLING );

	cam0->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam0->setPerspective( 35.0f, float( GAME_WIDTH ) / float( GAME_HEIGHT ), 1.0f, 10000.0f );
	cam0->setOrientAndPosition( Vect( 0.0f, 1.0f, 0.0f ), Vect( 0.0f, 0.0f, 10.0f ), Vect( 180.0f, 0.0f, 2000.0f ) );
	cam0->updateCamera();

	CameraMan::AddCamera( cam0 );
	GraphicsObjMan::AddDebugObject( cam0 );

	// create overview camera (watches camera0)
	auto cam1 = new CameraObject( CAMERA_OVERVIEW );

	cam1->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam1->setPerspective( 35.0f, float( GAME_WIDTH ) / float( GAME_HEIGHT ), 1.0f, 10000.0f );
	cam1->setOrientAndPosition( Vect( 0.0f, 0.0f, 1.0f ), Vect( 0.0f, 0.0f, 4.0f ), Vect( 500.0f, 0.0f, 4.0f ) );

	CameraMan::AddCamera( cam1 );

	CameraMan::SetCurrCamera( cam0 );

	// create the camera model
	auto myCameraModel = new CameraModel;
	myCameraModel->createVAO( cam0 );

	cam0->setCameraModel( myCameraModel );
}

void CameraMan::AddCamera( CameraObject* const inCamera ) {
	// get instance
	auto cMan = privGetInstance();

	// create a camera node
	auto node = new CameraNode;
	node->set( inCamera );

	// add to front of manager list
	cMan->privAddToFront( node, cMan->active );
}

void CameraMan::RemoveCamera() {
	// get instance
	auto cMan = privGetInstance();

	// remove current camera from the list
	auto walker = ( CameraNode * ) cMan->active;
	auto tmp = cMan->NextCamera();

	// do not remove the culling camera. i want that one at all times.
	if ( cMan->currCamera->getName() != CAMERA_CULLING ) {
		while ( walker != nullptr ) {
			if ( walker->myCamera == cMan->currCamera ) {
				cMan->privRemoveFromList( walker, cMan->active );
				cMan->SetCurrCamera( tmp );
			}
			walker = ( CameraNode * ) walker->next;
		}
	}
}

void CameraMan::DeleteCameras() {
	auto walker = ( CameraNode * ) privGetInstance()->active;
	auto tmp = walker;
	while ( walker != nullptr ) {
		walker = ( CameraNode* ) walker->next;
		delete tmp;
		tmp = walker;
	}
}

void CameraMan::SetCurrCamera( CameraObject* const inCam ) {
	privGetInstance()->currCamera = inCam;
}

void CameraMan::SwitchState() {
	auto cMan = privGetInstance();

	if ( cMan->camState == ORBIT ) {
		cMan->camState = ROTATE;
	} else {
		cMan->camState = ORBIT;
	}

	out( "Camera view state changed.\n" );
}

CameraObject* CameraMan::Find( const CameraName inName ) {
	// find the camera node
	auto walker = ( CameraNode * ) privGetInstance()->active;

	while ( walker != nullptr ) {
		if ( walker->myCamera->getName() == inName ) {
			break;
		}
		walker = ( CameraNode * ) walker->next;
	}

	if ( walker == nullptr ) {
		walker = ( CameraNode * ) privGetInstance()->active;
	}

	return walker->myCamera;
}

CameraObject* CameraMan::GetCurrCamera() {
	return privGetInstance()->currCamera;
}

CameraObject* CameraMan::NextCamera() {
	auto cMan = CameraMan::privGetInstance();

	// return next camera in list
	auto walker = ( CameraNode* ) cMan->currCamera;

	if ( walker->next != nullptr ) {
		walker = ( CameraNode* ) walker->next;
	} else {
		walker = ( CameraNode* ) cMan->active;
	}

	return walker->myCamera;
}

CameraState CameraMan::GetState() {
	return privGetInstance()->camState;
}

const void CameraMan::privAddToFront( CameraNodeLink* const node, CameraNodeLink*& head ) const {
	assert( node != nullptr );

	if ( head == nullptr ) {
		head = node;
	} else {
		node->next = head;
		head->prev = node;
		head = node;
	}
}

const void CameraMan::privRemoveFromList( CameraNodeLink* const node, CameraNodeLink *&head ) const {
	assert( node != nullptr );

	if ( node->prev == nullptr ) {
		if ( node->next == nullptr ) {
			// only in list
			head = nullptr;
		} else {
			// first in list
			node->next->prev = nullptr;
			head = node->next;
		}
	} else if ( node->next == nullptr ) {
		// last on list
		node->prev->next = nullptr;
	} else {
		// middle of list
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
}