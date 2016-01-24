#pragma once

class SphereObject;

class GenObject : public GraphicsObject
{
public:
	GenObject();
	
	// Generic object functions
	Vect getStartPos() const;
	const void setStartPos( const Vect& );
	const void setLightColor( const Vect& );
	const void setLightPos( const Vect& );
	const void setTextureName( const TextureName );
	const void setTextureName( const char* const );
	const void setStockShaderMode( const ShaderType );
	const void setOriginalSphere( const Sphere& );
	const void setSphereObject();
	const void setModel( Model* const );

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