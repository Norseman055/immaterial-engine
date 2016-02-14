#include "DEBUGGING.h"
#include "SphereModel.h"
#include "SphereObject.h"
#include "CameraManager.h"
#include "ModelManager.h"

extern GLShaderManager shaderManager;

// constructor
SphereObject::SphereObject()
	: GraphicsObject() {
	this->Shading = Shader_Texture_NoLights;
	this->cullingSphere = this;
};

void SphereObject::setPos( const Vect& v ) {
	this->sphere.cntr.set( v );
}

void SphereObject::setRad( const float radius ) {
	this->sphere.rad = radius;
}

void SphereObject::setStartPos( const Vect& v ) {
	this->startPos.set( v );
};

Vect SphereObject::getStartPos() {
	return this->startPos;
};

void SphereObject::setLightColor( const Vect& v ) {
	this->lightColor.set( v );
};

void SphereObject::setLightPos( const Vect& v ) {
	this->lightPos.set( v );
};

void SphereObject::setExtSphere( const Sphere& mySphere ) {
	this->extSphere = mySphere;
}

void SphereObject::setExtMatrix( const Matrix& world ) {
	this->extWorld.set( world );
}

void SphereObject::setTextureName( const TextureName inName ) {
	this->Texture = inName;
};

void SphereObject::setStockShaderMode( const ShaderType inVal ) {
	this->Shading = inVal;
};

Sphere SphereObject::getSphere() const {
	return this->sphere;
}

void SphereObject::checkCulling() { }

void SphereObject::transform() {
	// FIRST you need to match the Original Sphere on to the original Graphics Object

	// create temp matrices
	Matrix Trans( TRANS, this->extSphere.cntr[x], this->extSphere.cntr[y], this->extSphere.cntr[z] );

	float scale = 2.0f;
	// since the base model has a radius 0.5, so everything needs to multiplied by 2
	Matrix Scale( SCALE, this->extSphere.rad*scale, this->extSphere.rad*scale, this->extSphere.rad*scale );

	// Create the local to world matrix (ie Model)
	auto MapSphere = Scale * Trans;

	// Now apply the extern object so it moves and rotates the same
	this->World = MapSphere * this->extWorld;

	this->ModelView = this->World * CameraMan::GetCurrCamera()->getViewMatrix();
};

void SphereObject::setRenderState() {
	// Bind the texture
	GLuint textureID = TextureMan::Find( this->Texture );
	assert( textureID != -1 );
	glBindTexture( GL_TEXTURE_2D, textureID );

	CameraObject *cam = CameraMan::GetCurrCamera();
	Matrix mvp = this->ModelView * cam->getProjMatrix();

	// set the shader
	shaderManager.UseStockShader( GLT_SHADER_FLAT,
								  &mvp,
								  &this->lightColor );

	// set render states
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_CULL_FACE );
};

void SphereObject::draw() {
	glBindVertexArray( ModelMan::Find( "sphere" )->vao );
	glDrawElements( GL_TRIANGLES, 200 * 3, GL_UNSIGNED_SHORT, 0 );
};