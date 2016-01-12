#ifndef PYRAMID_OBJECT_H
#define PYRAMID_OBJECT_H

#include "GraphicsObject.h"
#include "TextureNode.h"
#include "ShaderTypes.h"
#include "SphereObject.h"
#include "PyramidModel.h"

class AnimController;

class PyramidObject : public GraphicsObject
{
public:
	PyramidObject();
	PyramidObject( const char * const _name );
	
	void setControllerDepthFirst(AnimController *const inController);
	
	// Specific to Pyramid
	void setStartPos( const Vect & v);
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setTextureName( TextureName inName );
	void setStockShaderMode ( ShaderType inVal );
	void setOriginalSphere( Sphere &origSphere );
	void setSphereObject ();
	void setModel(PyramidModel * const inModel);
	void setController(AnimController * const inController);
	void setScale(float inS);

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);
	void checkCulling(void);
	Vect getStartPos();

	// bring out into new type of object: animation object? remove from graphic object entirely
	void setIndex(int val);

private:
	// internal variables for the instance
	PyramidModel *model;
	SphereObject *sphereObj;
	AnimController *controller;
	int indexBoneArray;
	float scale;
};

#endif