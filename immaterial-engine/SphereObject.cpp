#include "OpenGL.h"
#include "DEBUGGING.h"

#include "SphereModel.h"
#include "MathEngine.h"
#include "SphereObject.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "CameraObject.h"
#include "ModelManager.h"

extern GLShaderManager shaderManager;

// constructor
SphereObject :: SphereObject()
{  
	this->angle_z = 0; 
	this->angle_y = 0; 

	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->Texture = NOT_INITIALIZED;

	this->sphere.rad = 1.0f;
	this->sphere.cntr = Vect(0.0f, 0.0f, 0.0f, 0.0f);

	this->Shading = Shader_Texture_NoLights;

	this->cullingSphere = this;
};

void SphereObject::setPos( const Vect &v )
{
	this->sphere.cntr = v;
}

void SphereObject::setRad( const float radius )
{
	this->sphere.rad = radius;
}

void SphereObject::setStartPos( const Vect & v)
{
	this->startPos = v;
};

Vect SphereObject::getStartPos()
{
	return this->startPos;
};

void SphereObject::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void SphereObject::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void SphereObject::setExtSphere( const Sphere &mySphere )
{
	this->extSphere = mySphere;
}

void SphereObject::setExtMatrix( const Matrix &world )
{
	this->extWorld = world;
}

void SphereObject::setTextureName( TextureName inName )
{
	this->Texture = inName;
};

void SphereObject::setStockShaderMode( ShaderType inVal )
{
	this->Shading  = inVal;
};

Sphere SphereObject::getSphere()
{
	return this->sphere;
}

void SphereObject::checkCulling(void)
{
}

void SphereObject::transform( void )
{
	// FIRST you need to match the Original Sphere on to the original Graphics Object
	
	// create temp matrices
		Matrix Trans( TRANS, this->extSphere.cntr[x], this->extSphere.cntr[y],this->extSphere.cntr[z]);

		float scale = 2.0f;
		// since the base model has a radius 0.5, so everything needs to multiplied by 2
		Matrix Scale( SCALE, this->extSphere.rad*scale, this->extSphere.rad*scale, this->extSphere.rad*scale);
	
	// Create the local to world matrix (ie Model)
		Matrix  MapSphere =  Scale * Trans;

	// Now apply the extern object so it moves and rotates the same
	this->World = MapSphere * this->extWorld;

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
	CameraObject *cam = CameraMan::GetCurrCamera();
	this->ModelView = this->World * cam->getViewMatrix();

};


void SphereObject::setRenderState( void )
{	
	// Bind the texture
	GLuint textureID = TextureMan::Find( this->Texture );
	glBindTexture(GL_TEXTURE_2D, textureID);

	CameraObject *cam = CameraMan::GetCurrCamera();

	// set the shader
		// modelViewProj matrix, stock shader.
			Matrix mvp = this->ModelView * cam->getProjMatrix();
			shaderManager.UseStockShader( GLT_SHADER_FLAT,
											&mvp,
											&this->lightColor );

			// set render states
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_CULL_FACE);
};

void SphereObject::draw( void )
{   
	glBindVertexArray(ModelMan::Find("sphere")->vao);
	glDrawElements(GL_TRIANGLES, 200 * 3, GL_UNSIGNED_SHORT, 0);
};