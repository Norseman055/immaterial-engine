#pragma once

class SphereObject;

class GenObject : public GraphicsObject {
public:
	GenObject();

	// Generic object functions
	Vect getStartPos() const;
	void setStartPos( const Vect& );
	void setLightColor( const Vect& );
	void setLightPos( const Vect& );
	void setTextureName( const TextureName );
	void setTextureName( char* const );
	void setStockShaderMode( const ShaderType );
	void setOriginalSphere( const Sphere& );
	void setSphereObject() const;
	void setModel( Model* const );

	// must define, base class has abstract methods
	void transform( void ) override;
	void draw( void ) override;
	void setRenderState( void ) override;
	void checkCulling( void ) override;

private:
	GLuint  modelVAO;
	int		modelTri;

	SphereObject *sphereObj;
};