#include "GraphicsObject.h"

GraphicsObject::GraphicsObject()
	: angle_y(0.0f), angle_z(0.0f), Texture(NOT_INITIALIZED), Shading(Shader_Wireframe)
{
	this->World.set(IDENTITY);
	this->ModelView.set(IDENTITY);
	this->BoneOrientation.set(IDENTITY);
	this->Local.set(IDENTITY);

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f );
	this->lightPos.set( 1.0f, 1.0f, 0.0f );

	this->origSphere.cntr.set( 0.0f, 0.0f, 0.0f );
	this->origSphere.rad = 1.0f;
}

Matrix GraphicsObject::getWorld() const {
	return Matrix(this->World);
}

void GraphicsObject::setBoneOrientation( const Matrix& tmp ) {
	this->BoneOrientation = tmp;
}

Matrix GraphicsObject::getBoneOrientation() const {
	return Matrix(this->BoneOrientation);
}