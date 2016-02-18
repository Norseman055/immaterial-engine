#pragma once

#include "BoundingSphere.h"
#include "DEBUGGING.h"
#include "OpenGL.h"

class Model : public Align16 {
public:
	Model() 
		: vao( 0 ), numTri( 0 ) 
	{
		out( "Model(): -----------------\n" );
	}

	Sphere boundingVol;
	GLuint vao;
	int numTri;
};