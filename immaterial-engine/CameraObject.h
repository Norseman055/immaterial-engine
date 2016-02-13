#pragma once

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "CameraModel.h"

enum CameraName {
	CAMERA_CULLING,
	CAMERA_OVERVIEW,
	CAMERA_GENERIC,
	NO_CAMERA
};

enum CullResult {
	CULL_INSIDE,
	CULL_OUTSIDE
};

class CameraObject : public GraphicsObject {
public:
	CullResult CullTest( const Sphere& ) const;

	void transform( void ) override;
	void draw( void ) override;
	void setRenderState( void ) override;
	void checkCulling( void ) override;

	// Setup on single camera
	void setPerspective( const float, const float, const float, const float );
	void setViewport( const int, const int, const int, const int );
	void setOrientAndPosition( const Vect&, const Vect&, const Vect& );
	void setCameraModel( CameraModel* const );

	// update camera system
	void updateCamera( void );

	// Get the matrices for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	// accessors
	void getPos( Vect& ) const;
	void getDir( Vect& ) const;
	void getUp( Vect& ) const;
	void getLookAt( Vect& ) const;
	void getRight( Vect& ) const;

	CameraName getName() const;

	void setPos( const Vect& );
	Vect getStartPos() override;

	void getFieldOfView( float& ) const;
	void setFieldOfView( const float );

	void getNearDist( float& ) const;
	void setNearDist( const float );

	void getNearTopLeft( Vect& ) const;
	void getNearTopRight( Vect& ) const;
	void getNearBottomLeft( Vect& ) const;
	void getNearBottomRight( Vect& ) const;
	void getFarTopLeft( Vect& ) const;
	void getFarTopRight( Vect& ) const;
	void getFarBottomLeft( Vect& ) const;
	void getFarBottomRight( Vect& ) const;

	explicit CameraObject( CameraName name );
	CameraObject();

private:  // methods
	void privSetViewState() const;
	void privCalcPlaneHeightWidth();
	void privCalcFrustumVerts();
	void privCalcFrustumCollisionNormals();
	void privUpdateProjectionMatrix();
	void privUpdateViewMatrix();

private:  // data

	// Projection Matrix
	Matrix projMatrix;
	Matrix viewMatrix;

	// camera unit vectors (up, dir, right)
	Vect	vUp;
	Vect	vDir;
	Vect	vRight;
	Vect	vPos;
	Vect	vLookAt;

	// world space coords for viewing frustum
	Vect	nearTopLeft;
	Vect	nearTopRight;
	Vect	nearBottomLeft;
	Vect	nearBottomRight;
	Vect	farTopLeft;
	Vect	farTopRight;
	Vect	farBottomLeft;
	Vect	farBottomRight;

	// Normals of the frustum
	Vect	frontNorm;
	Vect	backNorm;
	Vect	rightNorm;
	Vect	leftNorm;
	Vect	topNorm;
	Vect	bottomNorm;

	// Define the frustum inputs
	float	nearDist;
	float	farDist;
	float	fovy;
	float	aspectRatio;

	// Screen size in world space
	float	near_height;
	float	near_width;
	float	far_height;
	float	far_width;

	// viewports
	int		viewport_x;
	int		viewport_y;
	int		viewport_width;
	int		viewport_height;

	// model
	CameraModel *model;

	// name
	CameraName name;
};