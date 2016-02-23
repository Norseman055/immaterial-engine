#pragma once

#include "OpenGL.h"

class Camera;

class CameraModel {
public:
	CameraModel();

	void createVAO( Camera* const );
	void update();

	GLuint vao;
};