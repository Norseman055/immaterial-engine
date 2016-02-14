#pragma once

#include "OpenGL.h"

class CameraObject;

class CameraModel {
public:
	CameraModel();

	void createVAO( CameraObject* const );
	void update();

	GLuint vao;
};