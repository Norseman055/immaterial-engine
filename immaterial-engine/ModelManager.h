#pragma once

#include "ModelNode.h"

struct MyVertex_stride {
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

struct MyTriList {
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
};

class ModelMan {
public:
	// loads model from a formatted file
	static void LoadModel( const char* const );
	static void LoadBufferedModel( unsigned char* const );
	static void DeleteModels();

	// find a specified model based on known model name (ex. "pyramid", "cube", etc.)
	static Model* Find( char* const );

private:
	// singleton
	static ModelMan *privGetInstance();
	ModelMan();
	void privAddToFront( ModelNodeLink* const, ModelNodeLink*& ) const;

private:
	ModelNodeLink *active;
};