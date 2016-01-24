#pragma once

#include "OpenGL.h"

class CameraObject;

class CameraModel {
public:
	CameraModel();

	const void createVAO( CameraObject* const );
	const void update();

	GLuint vao;
};