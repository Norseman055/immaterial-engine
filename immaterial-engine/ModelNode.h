#pragma once

#include "OpenGL.h"
#include "NodeLink.h"
#include "Model.h"

#define MODEL_NAME_SIZE 32

class ModelNode : public NodeLink<Model> {
public:
	ModelNode( const char * const inModelName, GLuint inHash, Model * inMod )
		: NodeLink( inMod ), hashName( inHash ) 
	{
		if ( strlen( inModelName ) < MODEL_NAME_SIZE ) {
			memcpy( this->modelName, inModelName, strlen( inModelName ) );
			this->modelName[strlen( inModelName )] = '\0';
		} else {
			memcpy( this->modelName, inModelName, MODEL_NAME_SIZE - 1 );
			this->modelName[MODEL_NAME_SIZE - 1] = '\0';
		};
	}

	// name of model from file
	char modelName[MODEL_NAME_SIZE];

	// hashed model name
	GLuint hashName;
};