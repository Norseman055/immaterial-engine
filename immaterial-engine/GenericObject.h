#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include "GraphicsObject.h"
#include "TextureNode.h"
#include "ShaderTypes.h"
#include "SphereObject.h"

class GenObject : public GraphicsObject
{
public:
	GenObject();
	
	// Generic object functions
	void setStartPos( const Vect & v);
	Vect getStartPos();
	void setLightColor( const Vect & v);
	void setLightPos( const Vect & v);
	void setTextureName( TextureName inName );
	void setTextureName( const char * const inName );
	void setStockShaderMode ( ShaderType inVal );
	void setOriginalSphere( Sphere &origSphere );
	void setSphereObject ();
	void setModel( Model * inModel );

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState(void);
	void checkCulling(void);

private:

	GLuint  modelVAO;
	int		modelTri;

	SphereObject *sphereObj;
};

#endif