#pragma once

#include "BoundingSphere.h"

class Model : public Align16 {
public:
	Model();

	Sphere boundingVol;
	GLuint vao;
	int numTri;
};