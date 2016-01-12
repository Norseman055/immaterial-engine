#ifndef SPHERE_OBJECT_H
#define SPHERE_OBJECT_H

#include "GraphicsObject.h"
#include "TextureNode.h"
#include "ShaderTypes.h"
#include "BoundingSphere.h"

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
	void setPos( const Vect & v );
	void setRad( const float radius );
	
	void setExtSphere( const Sphere &sphere );
	void setExtMatrix( const Matrix &world );

	void setGraphicsObjec( GraphicsObject *graphicsObject );

	// Specific to Sphere
	void setStartPos( const Vect & v);
	Vect getStartPos();
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setTextureName( TextureName inName );
	void setStockShaderMode ( ShaderType inVal );
	void setOriginalSphere( Sphere &origSphere );
	void setSphereObject ();
	Sphere getSphere();

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);
	void checkCulling(void);

private:
	// internal variables for the instance
	float	angle_y;
	float	angle_z;

	Vect color;
	Sphere sphere;
	
	Matrix extWorld;
	Sphere extSphere;

	SphereObject *cullingSphere;
};

#endif