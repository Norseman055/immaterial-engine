#include "OpenGL.h"
#include "DEBUGGING.h"

#include "PyramidModel.h"
#include "PyramidObject.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "CameraObject.h"
#include "SphereObject.h"
#include "GraphicsManager.h"
#include "ModelManager.h"
#include "AnimationManager.h"
#include "Anim.h"
#include "AnimController.h"

extern GLShaderManager	shaderManager;
extern PyramidModel *myPyramid;
extern Frame_Bucket *pHead;

void PyramidObject::setControllerDepthFirst( AnimController * const inCont )
{
	PyramidObject *myChild;

	this->controller = inCont;

	if (this->getChild() != 0)
	{
		myChild = (PyramidObject *)this->getChild();
		while (myChild != 0)
		{
			myChild->setControllerDepthFirst( inCont );
			myChild = (PyramidObject *)myChild->getSibling();
		}
	}
}

// Constructor make sure everything is initialized
PyramidObject :: PyramidObject()
{
	this->angle_z = 0.0f;
	this->angle_y = 0.0f;
  
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);

	this->origSphere.cntr = Vect(0.0f, 0.0f, 0.0f);
	this->origSphere.rad = 1.0f;

	this->Shading = Shader_Texture_PointLights;

	this->sphereObj = new SphereObject;
	this->model = 0;

	this->scale = 1.0f;
};

PyramidObject :: PyramidObject( const char * const _name )
{
	this->setName( _name );

	this->angle_z = 0.0f;
	this->angle_y = 0.0f;
  
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f);
	this->lightPos.set(1.0f, 1.0f, 0.0f);

	this->origSphere.cntr = Vect(0.0f, 0.0f, 0.0f);
	this->origSphere.rad = 1.0f;

	this->Shading = Shader_Texture_NoLights;

	this->sphereObj = new SphereObject;
	this->model = 0;

	this->scale = 1.0f;
}

void PyramidObject::setStartPos( const Vect & v)
{
	this->startPos = v;
	setSphereObject();
};

Vect PyramidObject::getStartPos()
{
	return this->startPos;
};

void PyramidObject::setLightColor( const Vect & v)
{
	this->lightColor = v;
};

void PyramidObject::setLightPos( const Vect & v)
{
	this->lightPos = v;
};

void PyramidObject::setOriginalSphere( Sphere &inObj )
{
	this->origSphere = inObj;
}

void PyramidObject::setSphereObject()
{
	sphereObj->setPos(this->startPos);
	sphereObj->setLightColor( Vect(0.0f, 1.0f, 0.0f, 1.0f) );
	sphereObj->setRad(this->origSphere.rad);
	GraphicsObjMan::addDebugObject(sphereObj);
}

void PyramidObject::setTextureName( TextureName inName )
{
	this->Texture = inName;
};

void PyramidObject::setStockShaderMode( ShaderType inVal )
{
	this->Shading  = inVal;
};

void PyramidObject::setController( AnimController * const inController )
{
	this->controller = inController;
}

void PyramidObject::checkCulling(void)
{
	CameraObject *tmp = CameraMan::Find(CAMERA_CULLING);
	if (tmp->CullTest(this->origSphere) == CULL_INSIDE)
		this->sphereObj->setLightColor( Vect (1.0f, 1.0f, 1.0f, 1.0f) );
	else
		this->sphereObj->setLightColor( Vect (1.0f, 0.0f, 0.0f, 0.0f) );
};

void PyramidObject::setModel(PyramidModel * const inModel)
{
	this->model = inModel;
}

void PyramidObject::setIndex( int val )
{
	this->indexBoneArray = val;
}

void PyramidObject::setScale( float val )
{
	this->scale = val;
}

void PyramidObject::transform( void )
{
	Matrix ParentWorld;

	if (this->getParent() == 0)	{
		ParentWorld.set(IDENTITY);
	}
	else	{
		PyramidObject *parentObj = (PyramidObject *) this->getParent();
		ParentWorld = parentObj->World;
	}

	Frame_Bucket *tmp = this->controller->result;
	Bone *bResult = tmp->pBone;

	Matrix T = Matrix( TRANS, bResult[indexBoneArray].T );
	Matrix S = Matrix( SCALE, bResult[indexBoneArray].S );
	Quat Q = bResult[indexBoneArray].Q;

	Matrix M = S * Q * T;

	this->Local = Matrix(SCALE, this->scale, this->scale, this->scale) * M;
	this->World = this->Local * ParentWorld;

	// push data to the bounding sphere to help it update again
	this->sphereObj->setExtSphere(this->origSphere);
	this->sphereObj->setExtMatrix(this->World);

	// Create the ModelView ( LocalToWorld * View)
	CameraObject *cam = CameraMan::GetCurrCamera();
	this->ModelView = this->BoneOrientation * cam->getViewMatrix();
};


void PyramidObject::setRenderState( void )
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

void PyramidObject::draw( void )
{   
	glBindVertexArray(myPyramid->vao);
	glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, 0);
};