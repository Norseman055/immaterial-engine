#pragma once

#include "SphereObject.h"
#include "PyramidModel.h"

class AnimController;

class PyramidObject : public GraphicsObject {
public:
	PyramidObject();
	explicit PyramidObject( const char* const );
	PyramidObject( const char* const, const int );

	void setControllerDepthFirst( const AnimController * const inController );

	// Specific to Pyramid
	void setStartPos( const Vect& );
	void setLightColor( const Vect& );
	void setLightPos( const Vect& );
	void setTextureName( const TextureName );
	void setStockShaderMode( const ShaderType );
	void setOriginalSphere( const Sphere& );
	void setSphereObject() const;
	void setModel( PyramidModel* const );
	void setController( AnimController* const );
	void setScale( const float );

	// must define, base class has abstract methods
	void transform( void ) override;
	void draw( void ) override;
	void setRenderState( void ) override;
	void checkCulling( void ) override;
	Vect getStartPos() override;

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