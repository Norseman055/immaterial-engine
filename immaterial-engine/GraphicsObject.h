#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "PCSNode.h"
#include "ShaderTypes.h"
#include "TextureManager.h"
#include "BoundingSphere.h"

class GraphicsObject : public PCSNode, public Align16
{
public:
	 
	GraphicsObject();
	~GraphicsObject();

	// These are pure virtual, needed to be defined by the derived class
	virtual void transform( void ) = 0;   
	virtual void draw(void) = 0;
	virtual void setRenderState(void) = 0;
	virtual void checkCulling(void) = 0;

	virtual Vect getStartPos() = 0; 

	void setBoneOrientation( const Matrix &tmp );
	Matrix getBoneOrientation() const;

	Matrix getWorld() const;

protected:
	// variables
	float	angle_y;
	float	angle_z;

	Matrix	Local;
	Matrix	ModelView;
	Matrix	World;
	Matrix	BoneOrientation;

	ShaderType Shading;
	GLuint Texture;

	Vect startPos;
	Vect lightColor;
	Vect lightPos;

	Sphere origSphere;
};


#endif