#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CameraObject.h"
#include "CameraNode.h"
#include "CameraManager.h"
#include "GraphicsManager.h"

CameraMan::CameraMan()
	: active(0), camState(ORBIT)
{ }

// singleton
CameraMan * CameraMan::privGetInstance()
{
	static CameraMan cameraMan;
	return &cameraMan;
}

void CameraMan::LoadCameras()
{
	// Create camera0 (watches objects)
	CameraObject *cam0 = new CameraObject( CAMERA_CULLING );

	cam0->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam0->setPerspective( 35.0f, float(GAME_WIDTH)/float(GAME_HEIGHT), 1.0f, 10000.0f );
	cam0->setOrientAndPosition( Vect(0.0f, 1.0f,0.0f), Vect(0.0f,0.0f,10.0f), Vect(180.0f,0.0f,2000.0f) );
	cam0->updateCamera();

	CameraMan::addCamera( cam0 );
	GraphicsObjMan::addDebugObject( cam0 );

	// create overview camera (watches camera0)
	CameraObject *cam1 = new CameraObject ( CAMERA_OVERVIEW );

	cam1->setViewport( 0, 0, GAME_WIDTH, GAME_HEIGHT );
	cam1->setPerspective( 35.0f, float(GAME_WIDTH)/float(GAME_HEIGHT), 1.0f, 10000.0f );
	cam1->setOrientAndPosition( Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 4.0f), Vect(500.0f, 0.0f, 4.0f) );

	CameraMan::addCamera( cam1 );

	CameraMan::SetCurrCamera( cam0 );

	// create the camera model
	CameraModel* myCameraModel = new CameraModel();
	myCameraModel->createVAO( cam0 );

	cam0->setCameraModel(myCameraModel);
}

void CameraMan::addCamera( CameraObject *inCamera )
{
	// get instance
	CameraMan *cMan = CameraMan::privGetInstance();

	// create a camera node
	CameraNode *node = new CameraNode();
	node->set( inCamera );

	// add to front of manager list
	cMan->privAddToFront( node, cMan->active );
}

void CameraMan::removeCamera( )
{
	// get instance
	CameraMan *cMan = CameraMan::privGetInstance();

	// remove current camera from the list
	CameraNode *walker = (CameraNode *)cMan->active;
	CameraObject *tmp = cMan->NextCamera();

	// do not remove the culling camera. i want that one at all times.
	if (cMan->currCamera->getName() != CAMERA_CULLING)
	{
		while (walker->myCamera != cMan->currCamera)
		{
			walker = (CameraNode *)walker->next;
		}
		
		cMan->privRemoveFromList(walker, cMan->active);
		cMan->SetCurrCamera(tmp);
	}
}

CameraObject * CameraMan::Find( CameraName inName )
{
	// get instance
	CameraMan *cMan = CameraMan::privGetInstance();

	// find the camera node
	CameraNode *walker = (CameraNode *)cMan->active;
	while ( walker != 0 )
	{
		if (walker->myCamera->getName() == inName )
		{
			break;
		}
		walker = (CameraNode *)walker->next;
	}
	return walker->myCamera;
}

void CameraMan::SetCurrCamera( CameraObject * inCam )
{
	CameraMan *cMan = CameraMan::privGetInstance();
	cMan->currCamera = inCam;
}

CameraObject * CameraMan::GetCurrCamera()
{
	CameraMan *cMan = CameraMan::privGetInstance();
	return cMan->currCamera;
}

CameraObject * CameraMan::NextCamera()
{
	CameraMan *cMan = CameraMan::privGetInstance();
	CameraNode *retCam = (CameraNode *)cMan->active;
	
	// return next camera in list
	CameraNode *walker = (CameraNode *)cMan->active;
	while(walker->myCamera != cMan->currCamera)
	{
		walker = (CameraNode *)walker->next;
	}
	
	if( walker->next != 0 )
	{
		walker = (CameraNode *)walker->next;
		retCam = walker;
	}

	return retCam->myCamera;
}

void CameraMan::privAddToFront( CameraNodeLink *node, CameraNodeLink *&head )
{
	assert (node != 0);

	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void CameraMan::privRemoveFromList( CameraNodeLink *node, CameraNodeLink *&head )
{
	assert (node != 0);
	
	// first in list
	if (node->prev == 0)
	{
		// only in list
		if (node->next == 0)
		{
			head = 0;
		}
		else
		{
			node->next->prev = 0;
			head = node->next;
		}
	}
	// last on list
	else if (node->next == 0)
	{
		node->prev->next = 0;
	}
	// middle of list
	else
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
}

CameraState CameraMan::getState()
{
	CameraMan *cMan = CameraMan::privGetInstance();
	return cMan->camState;
}

void CameraMan::switchState()
{
	CameraMan *cMan = CameraMan::privGetInstance();

	if(cMan->camState == ORBIT)
	{
		cMan->camState = ROTATE;
	}
	else
	{
		cMan->camState = ORBIT;
	}
	printf("Camera view state changed.\n");
}