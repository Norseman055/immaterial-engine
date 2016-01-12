#ifndef TORUS_OBJECT_H
#define TORUS_OBJECT_H

#include "GraphicsObject.h"
#include "TextureNode.h"
#include "ShaderTypes.h"
#include "SphereObject.h"

class TorusObject : public GraphicsObject
{
public:
	TorusObject();

	// Specific to Sphere
	void setStartPos( const Vect & v);
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setTextureName( TextureName inName );
	void setStockShaderMode ( ShaderType inVal );
	void setOriginalSphere( Sphere &origSphere );
	void setSphereObject ();

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);
	void checkCulling(void);

private:
	// internal variables for the instance
	float	angle_y;
	float	angle_z;

	SphereObject *sphereObj;
};

#endif