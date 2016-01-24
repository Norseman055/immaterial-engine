#include "DEBUGGING.h"
#include "CameraObject.h"
#include "CameraManager.h"

extern GLShaderManager shaderManager;

CullResult CameraObject::CullTest( const Sphere& sphere ) {
	auto result = CULL_INSIDE;

	// first test
	auto A = sphere.cntr - this->nearTopLeft;

	if ( (A.dot( this->topNorm ) > sphere.rad) ||
		 (A.dot( this->leftNorm ) > sphere.rad) ||
		 (A.dot( this->frontNorm ) > sphere.rad) ) {
		result = CULL_OUTSIDE;
	} else {
		auto B = sphere.cntr - this->farBottomRight;

		if ( (B.dot( this->backNorm ) > sphere.rad) ||
			 (B.dot( this->rightNorm ) > sphere.rad) ||
			 (B.dot( this->bottomNorm ) > sphere.rad) ) {
			result = CULL_OUTSIDE;
		}
	}

	return result;
}

void CameraObject::transform( void ) {
	if ( this->model != nullptr ) {
		this->model->update();
	}

	// create the modelView
	this->ModelView = this->World * CameraMan::GetCurrCamera()->getViewMatrix();
}

void CameraObject::setRenderState() {
	// need model view projection matrix
	auto mvp = this->ModelView * CameraMan::GetCurrCamera()->getProjMatrix();

	// stock shader, wireframe
	shaderManager.UseStockShader( GLT_SHADER_FLAT,
								  &mvp,
								  &this->lightColor );

	// set render states
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_CULL_FACE );
}

void CameraObject::draw() {
	if ( this->model != nullptr ) {
		glBindVertexArray( this->model->vao );
		glDrawElements( GL_TRIANGLES, 8 * 3, GL_UNSIGNED_SHORT, 0 );
	}
}

void CameraObject::checkCulling() { }

// critical informational knobs for the perspective matrix
// Field of View Y is in degrees (copying lame OpenGL)
const void CameraObject::setPerspective( const float Fovy, const float Aspect, const float NearDist, const float FarDist ) {
	this->aspectRatio = Aspect;
	this->fovy = Fovy;
	this->nearDist = NearDist;
	this->farDist = FarDist;
};

// Just a pass through to setup the view port screen sub window
const void CameraObject::setViewport( const int inX, const int inY, const int width, const int height ) {
	this->viewport_x = inX;
	this->viewport_y = inY;
	this->viewport_width = width;
	this->viewport_height = height;

	this->privSetViewState();
};

// Simple wrapper
const void CameraObject::privSetViewState() {
	glViewport( this->viewport_x, this->viewport_y, this->viewport_width, this->viewport_height );
};

// Goal, calculate the near height / width, same for far plane
const void CameraObject::privCalcPlaneHeightWidth() {
	this->near_height = 2.0f * tanf( (this->fovy * MATH_PI / 180.0f) * .5f ) * this->nearDist;
	this->near_width = this->near_height * this->aspectRatio;

	this->far_height = 2.0f * tanf( (this->fovy * MATH_PI / 180.0f) * .5f ) * this->farDist;
	this->far_width = this->far_height * this->aspectRatio;
};

const void CameraObject::setOrientAndPosition( const Vect &inUp, const Vect &inLookAt, const Vect &inPos ) {
	this->vLookAt = inLookAt;

	// Point out of the screen into your EYE
	this->vDir = inPos - inLookAt;
	this->vDir.norm();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross( this->vDir );
	this->vRight.norm();

	this->vUp = this->vDir.cross( this->vRight );
	this->vUp.norm();

	this->vPos = inPos;
};

const void CameraObject::privCalcFrustumVerts() {
	this->nearTopLeft = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearTopRight = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->nearBottomLeft = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearBottomRight = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->farTopLeft = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farTopRight = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
	this->farBottomLeft = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farBottomRight = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
};

const void CameraObject::privCalcFrustumCollisionNormals() {
	// Normals of the frustum around nearTopLeft
	auto A = this->nearBottomLeft - this->nearTopLeft;
	auto B = this->nearTopRight - this->nearTopLeft;
	auto C = this->farTopLeft - this->nearTopLeft;

	this->frontNorm = A.cross( B );
	this->frontNorm.norm();

	this->leftNorm = C.cross( A );
	this->leftNorm.norm();

	this->topNorm = B.cross( C );
	this->topNorm.norm();

	// Normals of the frustum around farBottomRight
	A = this->farBottomLeft - this->farBottomRight;
	B = this->farTopRight - this->farBottomRight;
	C = this->nearBottomRight - this->farBottomRight;

	this->backNorm = A.cross( B );
	this->backNorm.norm();

	this->rightNorm = B.cross( C );
	this->rightNorm.norm();

	this->bottomNorm = C.cross( A );
	this->bottomNorm.norm();
};

// The projection matrix (note it's invertable)
const void CameraObject::privUpdateProjectionMatrix() {
	this->projMatrix[m0] = 2.0f * this->nearDist / this->near_width;
	this->projMatrix[m1] = 0.0f;
	this->projMatrix[m2] = 0.0f;
	this->projMatrix[m3] = 0.0f;

	this->projMatrix[m4] = 0.0f;
	this->projMatrix[m5] = 2.0f * this->nearDist / this->near_height;
	this->projMatrix[m6] = 0.0f;
	this->projMatrix[m7] = 0.0f;

	this->projMatrix[m8] = 0.0f;
	this->projMatrix[m9] = 0.0f;
	this->projMatrix[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
	this->projMatrix[m11] = -1.0f;

	this->projMatrix[m12] = 0.0f;
	this->projMatrix[m13] = 0.0f;
	this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
	this->projMatrix[m15] = 0.0f;
};

const void CameraObject::privUpdateViewMatrix() {
	// This functions assumes the your vUp, vRight, vDir are still unit
	// And perpendicular to each other
	// view = Rot(orient) * trans(-(eye.basis) )

	this->viewMatrix[m0] = this->vRight[x];
	this->viewMatrix[m1] = this->vUp[x];
	this->viewMatrix[m2] = this->vDir[x];
	this->viewMatrix[m3] = 0.0f;

	this->viewMatrix[m4] = this->vRight[y];
	this->viewMatrix[m5] = this->vUp[y];
	this->viewMatrix[m6] = this->vDir[y];
	this->viewMatrix[m7] = 0.0f;

	this->viewMatrix[m8] = this->vRight[z];
	this->viewMatrix[m9] = this->vUp[z];
	this->viewMatrix[m10] = this->vDir[z];
	this->viewMatrix[m11] = 0.0f;

	// Change of basis (dot with the basis vectors)
	this->viewMatrix[m12] = -vPos.dot( vRight );
	this->viewMatrix[m13] = -vPos.dot( vUp );
	this->viewMatrix[m14] = -vPos.dot( vDir );
	this->viewMatrix[m15] = 1.0f;
};

// Update everything (make sure it's consistent)
const void CameraObject::updateCamera() {
	// First find the near height/width, far height/width
	this->privCalcPlaneHeightWidth();

	// Find the frustum physical verts
	this->privCalcFrustumVerts();

	// find the frustum collision normals
	this->privCalcFrustumCollisionNormals();

	// update the projection matrix
	this->privUpdateProjectionMatrix();

	// update the view matrix
	this->privUpdateViewMatrix();
};

// Accessor mess:
Matrix& CameraObject::getViewMatrix() {
	return this->viewMatrix;
};

Matrix& CameraObject::getProjMatrix() {
	return this->projMatrix;
};

const void CameraObject::getPos( Vect &outPos ) const {
	outPos = this->vPos;
};

const void CameraObject::setPos( const Vect &inPos ) {
	this->vPos = inPos;
};

Vect CameraObject::getStartPos() {
	return this->vPos;
};

const void CameraObject::getDir( Vect &outDir ) const {
	outDir = this->vDir;
};

const void CameraObject::getUp( Vect &outUp ) const {
	outUp = this->vUp;
};

const void CameraObject::getLookAt( Vect &outLookAt ) const {
	outLookAt = this->vLookAt;
}

const void CameraObject::getRight( Vect &outRight ) const {
	outRight = this->vRight;
}

const void CameraObject::getFieldOfView( float &Value ) const {
	Value = this->aspectRatio;
};

const void CameraObject::setFieldOfView( const float Value ) {
	this->aspectRatio = Value;
};

const void CameraObject::getNearDist( float &Value ) const {
	Value = this->nearDist;
};

const void CameraObject::setNearDist( const float Value ) {
	this->nearDist = Value;
};

const void CameraObject::getNearTopLeft( Vect &vOut ) const {
	vOut = this->nearTopLeft;
};

const void CameraObject::getNearTopRight( Vect &vOut ) const {
	vOut = this->nearTopRight;
};

const void CameraObject::getNearBottomLeft( Vect &vOut )const {
	vOut = this->nearBottomLeft;
};

const void CameraObject::getNearBottomRight( Vect &vOut ) const {
	vOut = this->nearBottomRight;
};

const void CameraObject::getFarTopLeft( Vect &vOut ) const {
	vOut = this->farTopLeft;
};

const void CameraObject::getFarTopRight( Vect &vOut ) const {
	vOut = this->farTopRight;
};

const void CameraObject::getFarBottomLeft( Vect &vOut ) const {
	vOut = this->farBottomLeft;
};

const void CameraObject::getFarBottomRight( Vect &vOut )const {
	vOut = this->farBottomRight;
};

CameraName CameraObject::getName() const {
	return this->name;
};

const void CameraObject::setCameraModel( CameraModel* const inModel ) {
	this->model = inModel;
};

CameraObject::CameraObject( const CameraName inName )
	: CameraObject() {
	this->name = inName;
}

CameraObject::CameraObject()
	: name( CAMERA_GENERIC ), model( nullptr ) {
	this->World.set( IDENTITY );
	this->ModelView.set( IDENTITY );
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f );
	this->vLookAt.set( 0.0f, 0.0f, 0.0f );
}