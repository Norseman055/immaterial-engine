#include "Camera.h"
#include "CameraManager.h"

extern GLShaderManager shaderManager;

CullResult Camera::CullTest( const Sphere& sphere ) const {
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

void Camera::transform( void ) {
	if ( this->model != nullptr ) {
		this->model->update();
	}

	// create the modelView
	this->ModelView = this->World * CameraManager::GetCurrentCamera()->getViewMatrix();
}

void Camera::setRenderState() {
	// need model view projection matrix
	auto mvp = this->ModelView * CameraManager::GetCurrentCamera()->getProjMatrix();

	// stock shader, wireframe
	shaderManager.UseStockShader( GLT_SHADER_FLAT,
								  &mvp,
								  &this->lightColor );

	// set render states
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_CULL_FACE );
}

void Camera::draw() {
	if ( this->model != nullptr ) {
		glBindVertexArray( this->model->vao );
		glDrawElements( GL_TRIANGLES, 8 * 3, GL_UNSIGNED_SHORT, nullptr );
	}
}

void Camera::checkCulling() { }

// critical informational knobs for the perspective matrix
// Field of View Y is in degrees (copying lame OpenGL)
void Camera::setPerspective( const float Fovy, const float Aspect, const float NearDist, const float FarDist ) {
	this->aspectRatio = Aspect;
	this->fovy = Fovy;
	this->nearDist = NearDist;
	this->farDist = FarDist;
};

// Just a pass through to setup the view port screen sub window
void Camera::setViewport( const int inX, const int inY, const int width, const int height ) {
	this->viewport_x = inX;
	this->viewport_y = inY;
	this->viewport_width = width;
	this->viewport_height = height;

	this->privSetViewState();
};

// Simple wrapper
void Camera::privSetViewState() const {
	glViewport( this->viewport_x, this->viewport_y, this->viewport_width, this->viewport_height );
};

// Goal, calculate the near height / width, same for far plane
void Camera::privCalcPlaneHeightWidth() {
	this->near_height = 2.0f * tanf( (this->fovy * MATH_PI / 180.0f) * .5f ) * this->nearDist;
	this->near_width = this->near_height * this->aspectRatio;

	this->far_height = 2.0f * tanf( (this->fovy * MATH_PI / 180.0f) * .5f ) * this->farDist;
	this->far_width = this->far_height * this->aspectRatio;
};

void Camera::setOrientAndPosition( const Vect &inUp, const Vect &inLookAt, const Vect &inPos ) {
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

void Camera::privCalcFrustumVerts() {
	this->nearTopLeft = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearTopRight = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->nearBottomLeft = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearBottomRight = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->farTopLeft = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farTopRight = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
	this->farBottomLeft = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farBottomRight = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
};

void Camera::privCalcFrustumCollisionNormals() {
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

// The projection matrix (note it's invertible)
void Camera::privUpdateProjectionMatrix() {
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

void Camera::privUpdateViewMatrix() {
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
void Camera::updateCamera() {
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
Matrix& Camera::getViewMatrix() {
	return this->viewMatrix;
};

Matrix& Camera::getProjMatrix() {
	return this->projMatrix;
};

void Camera::getPos( Vect &outPos ) const {
	outPos = this->vPos;
};

void Camera::setPos( const Vect &inPos ) {
	this->vPos = inPos;
};

Vect Camera::getStartPos() {
	return this->vPos;
};

void Camera::getDir( Vect &outDir ) const {
	outDir = this->vDir;
};

void Camera::getUp( Vect &outUp ) const {
	outUp = this->vUp;
};

void Camera::getLookAt( Vect &outLookAt ) const {
	outLookAt = this->vLookAt;
}

void Camera::getRight( Vect &outRight ) const {
	outRight = this->vRight;
}

void Camera::getFieldOfView( float &Value ) const {
	Value = this->aspectRatio;
};

void Camera::setFieldOfView( const float Value ) {
	this->aspectRatio = Value;
};

void Camera::getNearDist( float &Value ) const {
	Value = this->nearDist;
};

void Camera::setNearDist( const float Value ) {
	this->nearDist = Value;
};

void Camera::getNearTopLeft( Vect &vOut ) const {
	vOut = this->nearTopLeft;
};

void Camera::getNearTopRight( Vect &vOut ) const {
	vOut = this->nearTopRight;
};

void Camera::getNearBottomLeft( Vect &vOut )const {
	vOut = this->nearBottomLeft;
};

void Camera::getNearBottomRight( Vect &vOut ) const {
	vOut = this->nearBottomRight;
};

void Camera::getFarTopLeft( Vect &vOut ) const {
	vOut = this->farTopLeft;
};

void Camera::getFarTopRight( Vect &vOut ) const {
	vOut = this->farTopRight;
};

void Camera::getFarBottomLeft( Vect &vOut ) const {
	vOut = this->farBottomLeft;
};

void Camera::getFarBottomRight( Vect &vOut )const {
	vOut = this->farBottomRight;
};

CameraName Camera::getName() const {
	return this->name;
};

void Camera::setCameraModel( CameraModel* const inModel ) {
	this->model = inModel;
};

Camera::Camera( const CameraName inName )
	: Camera() {
	this->name = inName;
}

Camera::Camera()
	: nearDist( 0 ), farDist( 0 ), fovy( 0 ), aspectRatio( 0 ), near_height( 0 ), near_width( 0 ), far_height( 0 ), far_width( 0 ),
	viewport_x( 0 ), viewport_y( 0 ), viewport_width( 0 ), viewport_height( 0 ), model( nullptr ), name( CAMERA_GENERIC ) {
	this->World.set( IDENTITY );
	this->ModelView.set( IDENTITY );
	this->lightColor.set( 1.0f, 1.0f, 1.0f, 1.0f );
	this->vLookAt.set( 0.0f, 0.0f, 0.0f );
}