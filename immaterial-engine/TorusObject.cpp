#include "OpenGL.h"
#include "DEBUGGING.h"

#include "TorusModel.h"
#include "MathEngine.h"
#include "TorusObject.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "CameraObject.h"
#include "SphereObject.h"
#include "GraphicsManager.h"
#include "ModelManager.h"

extern GLShaderManager shaderManager;

// constructor
TorusObject :: TorusObject()
{
	this->angle_z = (float)(rand()%100)/100.0f * MATH_PI;
	this->angle_y = (float)(rand()%100)/100.0f * MATH_PI;
  
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);
	this->Texture = NOT_INITIALIZED;

	this->origSphere.cntr = Vect(0.0f, 0.0f, 0.0f);
	this->origSphere.rad = 1.0f;

	this->LocalToWorld.set(IDENTITY);
	this->ModelView.set(IDENTITY);

	this->Shading = Shader_Texture_NoLights;

	this->sphereObj = new SphereObject;
};

void TorusObject::setStartPos( const Vect & v)
{
	this->startPos = v;
	setSphereObject();
};

void TorusObject::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void TorusObject::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void TorusObject::setTextureName( TextureName inName )
{
	this->Texture = inName;
};

void TorusObject::setOriginalSphere( Sphere &inObj )
{
	this->origSphere = inObj;
}

void TorusObject::setSphereObject()
{
	sphereObj->setPos(this->startPos);
	sphereObj->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f) );
	GraphicsObjMan::addObject(sphereObj);
}

void TorusObject::setStockShaderMode( ShaderType inVal )
{
	this->Shading  = inVal;
};

void TorusObject::checkCulling(void)
{
	CameraObject *tmp = CameraMan::Find(CAMERA_CULLING);
	if (tmp->CullTest(this->origSphere) == CULL_INSIDE)
		this->sphereObj->setLightColor( Vect (1.0f, 1.0f, 1.0f, 1.0f) );
	else
		this->sphereObj->setLightColor( Vect (1.0f, 0.0f, 0.0f, 0.0f) );
}

void TorusObject::transform( void )
{
   // update the angles
	angle_y += 0.04f;
	angle_z += 0.0006f;
	
	// create temp matrices
	Matrix RotY(ROT_Y, angle_y);
	Matrix Scale(SCALE, 1.0f,1.0f, 1.0f);
	Matrix RotZ(ROT_Z, angle_z);
	Matrix Trans( TRANS, this->startPos[x], this->startPos[y],this->startPos[z]);
	
	// Create the local to world matrix (ie Model)
	this->LocalToWorld = Scale * RotY * RotZ * Trans;

	// push data to the bounding sphere to help it update again
	this->sphereObj->setExtSphere(this->origSphere);
	this->sphereObj->setExtMatrix(this->LocalToWorld);

	// Create the ModelView ( LocalToWorld * View)
	// Some pipelines have the project concatenated, others don't
	// Best to keep the separated, you can always join them with a quick multiply
	CameraObject *cam = CameraMan::GetCurrCamera();
	this->ModelView = this->LocalToWorld * cam->getViewMatrix();

};


void TorusObject::setRenderState( void )
{	
	// Bind the texture
	GLuint textureID = TextureMan::Find( this->Texture );
	glBindTexture(GL_TEXTURE_2D, textureID);

	CameraObject *cam = CameraMan::GetCurrCamera();

	// set the shader
	switch ( this->Shading )
	{
	case Shader_Texture_PointLights:
		{
			// Use the stock shader
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
										&ModelView,
										&cam->getProjMatrix(), 
										&this->lightPos, 
										&this->lightColor, 
										0);

			// set render states
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		break;

	case Shader_Texture_NoLights:
		{
			// modelViewProj matrix, stock shader.
			Matrix mvp = this->ModelView * cam->getProjMatrix();
			shaderManager.UseStockShader( GLT_SHADER_TEXTURE_REPLACE,
											&mvp,
											0 );

			// set render states
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		break;

	case Shader_NoTexture_PointLights:
		{
			// use stock shaders
			shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, 
										&ModelView,
										&cam->getProjMatrix(), 
										&this->lightPos, 
										&this->lightColor);
		
			// set the render states
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		break;

	case Shader_Wireframe:
		{
			// modelViewProj matrix, stock shader.
			Matrix mvp = this->ModelView * cam->getProjMatrix();
			shaderManager.UseStockShader( GLT_SHADER_FLAT,
											&mvp,
											&this->lightColor );

			// set render states
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_CULL_FACE);
		}
		break;
	}

};

void TorusObject::draw( void )
{   
	glBindVertexArray(ModelMan::Find("torus")->vao);
	glDrawElements(GL_TRIANGLES, 220 * 3, GL_UNSIGNED_SHORT, 0);
};