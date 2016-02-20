#include "DEBUGGING.h"
#include "PyramidObject.h"
#include "CameraManager.h"
#include "CameraObject.h"
#include "GraphicsManager.h"
#include "AnimController.h"

extern GLShaderManager shaderManager;
extern PyramidModel *myPyramid;
extern Frame_Bucket *pHead;

void PyramidObject::setControllerDepthFirst( const AnimController * const inCont ) {
	PyramidObject *myChild;
	this->controller = inCont;

	if ( this->getChild() != nullptr ) {
		myChild = static_cast< PyramidObject * >(this->getChild());
		while ( myChild != nullptr ) {
			myChild->setControllerDepthFirst( inCont );
			myChild = static_cast< PyramidObject * >(myChild->getSibling());
		}
	}
}

// Constructor make sure everything is initialized
PyramidObject::PyramidObject()
	: GraphicsObject() {
	this->Texture = STONES;
	this->Shading = Shader_Texture_PointLights;

	this->sphereObj = new SphereObject;
	this->model = nullptr;
	this->controller = nullptr;
	this->indexBoneArray = -1;
	this->scale = 1.0f;
};

PyramidObject::PyramidObject( const char* const _name )
	: PyramidObject() {
	this->setName( _name );
}

PyramidObject::PyramidObject( const char* const _name, const int _index )
	: PyramidObject( _name ) {
	this->indexBoneArray = _index;
}

void PyramidObject::setStartPos( const Vect& v ) {
	this->startPos = v;
	setSphereObject();
};

Vect PyramidObject::getStartPos() {
	return this->startPos;
};

void PyramidObject::setLightColor( const Vect& v ) {
	this->lightColor = v;
};

void PyramidObject::setLightPos( const Vect& v ) {
	this->lightPos = v;
};

void PyramidObject::setOriginalSphere( const Sphere& inObj ) {
	this->origSphere = inObj;
}

void PyramidObject::setSphereObject() const {
	sphereObj->setPos( this->startPos );
	sphereObj->setLightColor( Vect( 0.0f, 1.0f, 0.0f, 1.0f ) );
	sphereObj->setRad( this->origSphere.rad );
	GraphicsObjMan::AddDebugObject( sphereObj );
}

void PyramidObject::setTextureName( const TextureName inName ) {
	this->Texture = inName;
};

void PyramidObject::setStockShaderMode( const ShaderType inVal ) {
	this->Shading = inVal;
};

void PyramidObject::setController( AnimController* const inController ) {
	this->controller = inController;
}

void PyramidObject::checkCulling() {
	auto tmp = CameraMan::Find( CAMERA_CULLING );
	if ( tmp->CullTest( this->origSphere ) == CULL_INSIDE ) {
		this->sphereObj->setLightColor( Vect( 1.0f, 1.0f, 1.0f, 1.0f ) );
	} else {
		this->sphereObj->setLightColor( Vect( 1.0f, 0.0f, 0.0f, 0.0f ) );
	}
};

void PyramidObject::setModel( PyramidModel* const inModel ) {
	this->model = inModel;
}

void PyramidObject::setIndex( const int val ) {
	this->indexBoneArray = val;
}

void PyramidObject::setScale( const float val ) {
	this->scale = val;
}

void PyramidObject::transform() {
	Matrix ParentWorld;

	if ( this->getParent() == nullptr ) {
		ParentWorld.set( IDENTITY );
	} else {
		auto parentObj = static_cast< PyramidObject * >(this->getParent());
		ParentWorld = parentObj->World;
	}

	auto tmp = this->controller->result;
	auto bResult = tmp->pBone;

	auto T = Matrix( TRANS, bResult[indexBoneArray].T );
	auto S = Matrix( SCALE, bResult[indexBoneArray].S );
	auto Q = bResult[indexBoneArray].Q;

	auto M = S * Q * T;

	this->Local = Matrix( SCALE, this->scale, this->scale, this->scale ) * M;
	this->World = this->Local * ParentWorld;

	// push data to the bounding sphere to help it update again
	this->sphereObj->setExtSphere( this->origSphere );
	this->sphereObj->setExtMatrix( this->World );

	// Create the ModelView ( LocalToWorld * View)
	this->ModelView = this->BoneOrientation * CameraMan::GetCurrCamera()->getViewMatrix();
};

void PyramidObject::setRenderState() {
	// Bind the texture
	auto textureID = TextureManager::Find( this->Texture );
	assert( textureID != -1 );
	glBindTexture( GL_TEXTURE_2D, textureID );

	auto cam = CameraMan::GetCurrCamera();
	auto mvp = this->ModelView * cam->getProjMatrix();

	// set the shader
	switch ( this->Shading ) {
		case Shader_Texture_PointLights:
			// Use the stock shader
			shaderManager.UseStockShader( GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
										  &this->ModelView,
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
			shaderManager.UseStockShader( GLT_SHADER_TEXTURE_REPLACE,
										  &mvp,
										  0 );

			// set render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		case Shader_NoTexture_PointLights:
			// use stock shaders
			shaderManager.UseStockShader( GLT_SHADER_POINT_LIGHT_DIFF,
										  &this->ModelView,
										  &cam->getProjMatrix(),
										  &this->lightPos,
										  &this->lightColor );

			// set the render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_CULL_FACE );
			break;
		default:
			// modelViewProj matrix, stock shader.
			shaderManager.UseStockShader( GLT_SHADER_FLAT,
										  &mvp,
										  &this->lightColor );

			// set render states
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glEnable( GL_CULL_FACE );
			break;
	}
};

void PyramidObject::draw() {
	glBindVertexArray( myPyramid->vao );
	glDrawElements( GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, nullptr );
};