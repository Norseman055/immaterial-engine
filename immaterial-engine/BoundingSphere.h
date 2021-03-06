#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "Vect.h"

struct Sphere : public Align16
{
	Vect cntr;
	float rad;
};

struct Vertex
{
		float x;
		float y;
		float z;
		float w;
};

#endif