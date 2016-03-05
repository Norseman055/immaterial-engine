#include "DEBUGGING.h"
#include "SphereObject.h"
#include "Model.h"
#include "GenericObject.h"
#include "CameraManager.h"
#include "ShaderManager.h"
#include "GraphicsManager.h"
#include "md5.h"

// constructor
GenObject::GenObject()
	: GraphicsObject(), modelVAO( 0 ) {
	this->Shading = Shader_Texture_NoLights;

	this->modelTri = 0;
	this->sphereObj = new SphereObject;
};

void GenObject::setStartPos( const Vect& v ) {
	this->startPos = v;
};

Vect GenObject::getStartPos() const {
	return this->startPos;
}

void GenObject::setLightColor( const Vect& v ) {
	this->lightColor = v;
};

void GenObject::setLightPos( const Vect& v ) {
	this->lightPos = v;
};

void GenObject::setOriginalSphere( const Sphere& inObj ) {
	this->origSphere = inObj;
	setSphereObject();
}

void GenObject::setSphereObject() const {
	sphereObj->setPos( this->startPos );
	sphereObj->setLightColor( Vect( 0.0f, 1.0f, 0.0f, 1.0f ) );
	sphereObj->setRad( this->origSphere.rad );
	GraphicsObjMan::AddDebugObject( sphereObj );
}

void GenObject::setModel( Model* const inModel ) {
	this->modelVAO = inModel->vao;
	this->modelTri = inModel->numTri;
	this->setOriginalSphere( inModel->boundingVol );
}

void GenObject::setTextureName( const TextureName inName ) {
	this->Texture = inName;
}

void GenObject::setTextureName( char* const inName ) {
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(inName), strlen( inName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	this->Texture = hashID;
};

void GenObject::setStockShaderMode( const ShaderType inVal ) {
	this->Shading = inVal;
};

void GenObject::checkCulling() {
	auto tmp = CameraManager::Find( CAMERA_CULLING );
	if ( tmp != nullptr ) {
		if ( tmp->CullTest( this->sphereObj->getSphere() ) == CULL_INSIDE )
			this->sphereObj->setLightColor( Vect( 0.0f, 1.0f, 0.0f, 0.0f ) );
		else
			this->sphereObj->setLightColor( Vect( 1.0f, 0.0f, 0.0f, 0.0f ) );
	}
}

void GenObject::transform() {
	// create temp matrices
	Matrix RotY( ROT_Y, angle_y );
	Matrix Scale( SCALE, 1.0f, 1.0f, 1.0f );
	Matrix RotZ( ROT_Z, angle_z );
	Matrix Trans( TRANS, this->startPos[x], this->startPos[y], this->startPos[z] );

	// Create the local to world matrix (ie Model)
	this->World = Scale * RotY * RotZ * Trans;

	// push data to the bounding sphere to help it update again
	this->sphereObj->setExtSphere( this->origSphere );
	this->sphereObj->setExtMatrix( this->World );

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
	this->ModelView = this->World * CameraManager::GetCurrentCamera()->getViewMatrix();
};

void GenObject::setRenderState() {
	// Bind the texture
	auto textureID = TextureManager::GetTextureID( this->Texture );
	assert( textureID != -1 );
	glBindTexture( GL_TEXTURE_2D, textureID );

	auto cam = CameraManager::GetCurrentCamera();
	auto mvp = this->ModelView * cam->getProjMatrix();

	// set the shader
	switch ( this->Shading ) {
		case Shader_Texture_PointLights:
			// Use the stock shader
			ShaderManager::GetGLShaderManager().UseStockShader( GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
										  &ModelView,
										  &cam->getProjMatrix(),
										  &this->lightPos,
										  &this->lightColor,
										  0 );

			// set render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		case Shader_Texture_NoLights:
			// modelViewProj matrix, stock shader.
			ShaderManager::GetGLShaderManager().UseStockShader( GLT_SHADER_TEXTURE_REPLACE,
										  &mvp,
										  0 );

			// set render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		case Shader_NoTexture_PointLights:
			// use stock shaders
			ShaderManager::GetGLShaderManager().UseStockShader( GLT_SHADER_POINT_LIGHT_DIFF,
										  &ModelView,
										  &cam->getProjMatrix(),
										  &this->lightPos,
										  &this->lightColor );

			// set the render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		case Shader_NoTexture_NoLights:
			// use stock shaders
			ShaderManager::GetGLShaderManager().UseStockShader( GLT_SHADER_FLAT,
										  &mvp,
										  &this->lightColor );
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		default:
			// modelViewProj matrix, stock shader.
			ShaderManager::GetGLShaderManager().UseStockShader( GLT_SHADER_FLAT,
										  &mvp,
										  &this->lightColor );

			// set render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glEnable( GL_CULL_FACE );
			break;
	}
};

void GenObject::draw() {
	glBindVertexArray( this->modelVAO );
	glDrawElements( GL_TRIANGLES, this->modelTri * 3, GL_UNSIGNED_SHORT, nullptr );
};