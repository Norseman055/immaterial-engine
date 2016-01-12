
#ifndef CAMERA_MODEL_H
#define CAMERA_MODEL_H

#include "OpenGL.h"

class CameraObject;

class CameraModel
{
public: 
	CameraModel();

	void createVAO( CameraObject *inCam );
	void update();

	GLuint vao;
};

#endif