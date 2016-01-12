#ifndef MODEL_H
#define MODEL_H

#include "BoundingSphere.h"

class Model : public Align16
{
public:

	Model();

	Sphere boundingVol;
	// placement for model's VAO handle
	GLuint vao;
	int numTri;
};

#endif