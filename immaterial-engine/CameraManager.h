#pragma once

#include "CameraNode.h"
#include "CameraObject.h"

enum CameraState {
	ORBIT,
	ROTATE
};

class CameraMan {
public:
	static void LoadCameras();
	static void AddCamera( CameraObject* const );
	static void RemoveCamera();
	static void DeleteCameras();
	static void SetCurrCamera( CameraObject* const );
	static void SwitchState();
	static CameraObject* Find( const CameraName );
	static CameraObject* GetCurrCamera();
	static CameraObject* NextCamera();
	static CameraState GetState();

private:
	static CameraMan* privGetInstance();
	CameraMan();
	void privAddToFront( CameraNodeLink* const, CameraNodeLink*& ) const;
	void privRemoveFromList( CameraNodeLink* const, CameraNodeLink*& ) const;

	CameraNodeLink *active;
	CameraObject *currCamera;
	CameraState camState;
};