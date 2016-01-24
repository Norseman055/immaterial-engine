
#include <string.h>

#include "DEBUGGING.h"
#include "ModelNode.h"

ModelNode::ModelNode()
	: hashName(0), storedModel(nullptr)
{
	// default constructor set name to zeroes
	memset( this->modelName, 0, MODEL_NAME_SIZE );
}

void ModelNode::set( const char * const inModelName,
						GLuint inHash, 
						Model * inMod)
{
	if (strlen(inModelName) < MODEL_NAME_SIZE)	{
		memcpy( this->modelName, inModelName, strlen(inModelName) );
		this->modelName[strlen(inModelName)] = '\0';
	}
	else	{
		memcpy( this->modelName, inModelName, MODEL_NAME_SIZE - 1 );
		this->modelName[MODEL_NAME_SIZE - 1] = '\0';
	}
	
	this->hashName = inHash;
	this->storedModel = inMod;
}