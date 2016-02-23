#pragma once

#include "GraphicsObject.h"

enum SphereMove {
	SphereMove_Y,
	SphereMove_Z,
	SphereMove_None
};

class SphereObject : public GraphicsObject {
public:
	SphereObject();

	// for bounding volumes
	void setPos( const Vect & v );
	void setRad( const float radius );
	void setExtSphere( const Sphere& );
	void setExtMatrix( const Matrix& );

	// Specific to Sphere
	void setStartPos( const Vect& );
	void setLightColor( const Vect& );
	void setLightPos( const Vect& );
	void setTextureName( const TextureName );
	void setStockShaderMode( const ShaderType );
	Sphere getSphere() const;

	// must define, base class has abstract methods
	Vect getStartPos() override;
	void transform( void ) override;
	void draw( void ) override;
	void setRenderState( void ) override;
	void checkCulling( void ) override;

private:
	// internal variables for the instance
	Matrix extWorld;
	Vect color;
	Sphere extSphere;
	Sphere sphere;
	SphereObject *cullingSphere;
};