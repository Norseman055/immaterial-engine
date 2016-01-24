#pragma once

#include "SphereObject.h"
#include "PyramidModel.h"

class AnimController;

class PyramidObject : public GraphicsObject {
public:
	PyramidObject();
	PyramidObject( const char* const );
	PyramidObject( const char* const, const int );

	const void setControllerDepthFirst( const AnimController * const inController );

	// Specific to Pyramid
	const void setStartPos( const Vect& );
	const void setLightColor( const Vect& );
	const void setLightPos( const Vect& );
	const void setTextureName( const TextureName );
	const void setStockShaderMode( const ShaderType );
	const void setOriginalSphere( const Sphere& );
	const void setSphereObject();
	const void setModel( PyramidModel* const );
	const void setController( AnimController* const );
	const void setScale( const float );

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState( void );
	void checkCulling( void );
	Vect getStartPos();

	// bring out into new type of object: animation object? remove from graphic object entirely
	void setIndex( int val );

private:
	// internal variables for the instance
	PyramidModel* model;
	SphereObject* sphereObj;
	const AnimController* controller;
	int indexBoneArray;
	float scale;
};