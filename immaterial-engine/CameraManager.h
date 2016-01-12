
#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "CameraNode.h"
#include "CameraObject.h"

enum CameraState
{
	ORBIT,
	ROTATE
};

class CameraMan
{
public:
	static void addCamera( CameraObject *inCamera );
	static void removeCamera( );
	static CameraObject * Find( const CameraName inName );
	static CameraObject * NextCamera();
	static void SetCurrCamera( CameraObject * inCam );
	static CameraObject * GetCurrCamera();
	static void switchState();
	static CameraState getState();
	static void LoadCameras();

private:
	static CameraMan * privGetInstance();
	CameraMan();
	void privAddToFront( CameraNodeLink *node, CameraNodeLink *&head );
	void privRemoveFromList( CameraNodeLink *node, CameraNodeLink *&head );

private:
	CameraNodeLink *active;
	CameraObject *currCamera;
	CameraState camState;
};

#endif