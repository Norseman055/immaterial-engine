#pragma once

#include "GraphicsObject.h"

enum SphereMove
{
	SphereMove_Y,
	SphereMove_Z,
	SphereMove_None
};

class SphereObject : public GraphicsObject
{
public:
	SphereObject();

	// for bounding volumes
	const void setPos( const Vect & v );
	const void setRad( const float radius );
	
	const void setExtSphere( const Sphere& );
	const void setExtMatrix( const Matrix& );

	const void setGraphicsObjec( GraphicsObject* const);

	// Specific to Sphere
	const void setStartPos( const Vect& );
	Vect getStartPos();
	const void setLightColor( const Vect& );
	const void setLightPos( const Vect& );
	const void setTextureName( const TextureName );
	const void setStockShaderMode( const ShaderType );
	const void setOriginalSphere( const Sphere& );
	const void setSphereObject ();
	Sphere getSphere() const;

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);
	void checkCulling(void);

private:
	// internal variables for the instance
	Matrix extWorld;
	Vect color;
	Sphere extSphere;
	Sphere sphere;
	SphereObject *cullingSphere;
};