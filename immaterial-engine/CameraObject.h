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

class CameraObject : public GraphicsObject
{
public:
	CullResult CullTest(const Sphere&);

	void transform(void);
	void draw(void);
	void setRenderState(void);
	void checkCulling(void);

	    // Setup on single camera
	const void setPerspective(const float, const float, const float, const float);
	const void setViewport(const int, const int, const int, const int);
	const void setOrientAndPosition(const Vect&, const Vect&, const Vect&);
	const void setCameraModel(CameraModel* const);

	 // update camera system
	const void updateCamera(void);

	// Get the matrices for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	// accessors
	const void getPos( Vect& ) const;
	const void getDir( Vect& ) const;
	const void getUp( Vect& ) const;
	const void getLookAt( Vect& ) const;
	const void getRight( Vect& ) const;

	CameraName getName() const;

	const void setPos( const Vect& );
	Vect getStartPos();
			
	const void getFieldOfView( float& ) const;
	const void setFieldOfView( const float );

	const void getNearDist( float& ) const;
	const void setNearDist( const float );


	const void getNearTopLeft( Vect& ) const;
	const void getNearTopRight( Vect& ) const;
	const void getNearBottomLeft( Vect& ) const;
	const void getNearBottomRight( Vect& ) const;	
	const void getFarTopLeft( Vect& ) const;
	const void getFarTopRight( Vect& ) const;
	const void getFarBottomLeft( Vect& ) const;
	const void getFarBottomRight( Vect& ) const;

	CameraObject( CameraName name );
	CameraObject();

private:  // methods
	const void privSetViewState();
	const void privCalcPlaneHeightWidth();
	const void privCalcFrustumVerts();
	const void privCalcFrustumCollisionNormals();
	const void privUpdateProjectionMatrix();
	const void privUpdateViewMatrix();

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