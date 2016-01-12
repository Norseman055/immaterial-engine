#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "OpenGL.h"
#include "ModelNode.h"

// models used in my engine:
/*
	space_frigate
	warbear_binary
	typhoon (NOT WORKING)
	buggy
	astroboy
	2BonePolySkinnedSkeleton

*/

struct MyVertex_stride
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

struct MyTriList
{
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
};

class ModelMan
{
public:
	// loads model from a formatted file
	static void LoadModel( const char * const fileName );
	static void LoadBufferedModel( const unsigned char * const modelBuff);
	static void DeleteModels();

	// find a specified model based on known model name (ex. "pyramid", "cube", etc.)
	static Model* Find( const char * const modelName );

private:
	// singleton
	static ModelMan *privGetInstance();
	ModelMan();
	void privAddToFront( ModelNodeLink *node, ModelNodeLink*&head );

private:
	ModelNodeLink *active;
};

#endif