#pragma once

#include "Vect.h"

struct Sphere : Align16 {
	Vect cntr;
	float rad;
};

struct Vertex {
	float x;
	float y;
	float z;
	float w;
};