#include "GraphicsObject.h"

GraphicsObject::GraphicsObject()
{
	this->World.set(IDENTITY);
	this->ModelView.set(IDENTITY);
	this->BoneOrientation.set(IDENTITY);
	this->Local.set(IDENTITY);

	this->Texture = STONES;
	this->Shading = Shader_Wireframe;
}

GraphicsObject::~GraphicsObject()
{
}

Matrix GraphicsObject::getWorld( ) const
{
	return Matrix(this->World);
}

void GraphicsObject::setBoneOrientation( const Matrix &tmp )
{
	this->BoneOrientation = tmp;
}

Matrix GraphicsObject::getBoneOrientation( ) const
{
	return Matrix(this->BoneOrientation);
}