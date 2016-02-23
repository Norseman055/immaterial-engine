#pragma once

#include "CameraNode.h"
#include "Manager.h"

enum CameraState {
	ORBIT,
	ROTATE
};

class CameraManager : public Manager<Camera> {
public:
	// Functions
	static void LoadCameras();

	static Camera* GetCurrentCamera();
	static void SetCurrentCamera( Camera* const );
	static void RemoveCurrentCamera();

	static CameraState GetState();
	static void SwitchState();

	static Camera* Find( const CameraName );
	static Camera* GetNextCamera();

private:
	// Functions
	static CameraManager* privGetInstance();
	CameraManager();

	// Data
	Camera *currentCamera;
	CameraState cameraState;
};