#pragma once

#include "OpenGL.h"
#include "Model.h"

#define MODEL_NAME_SIZE 32

class ModelNodeLink {
public:
	ModelNodeLink()
		: next( nullptr ), prev( nullptr ) { }

	ModelNodeLink *next;
	ModelNodeLink *prev;
};

class ModelNode : public ModelNodeLink {
public:
	ModelNode();
	void set( const char * const inModelName,
			  GLuint inHash,
			  Model * inMod );

	// name of model from file
	char modelName[MODEL_NAME_SIZE];

	// hashed model name
	GLuint hashName;

	// VAO reference to model in GPU
	Model *storedModel;
};