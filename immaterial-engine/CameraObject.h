
#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "BoundingSphere.h"
#include "CameraModel.h"

 // now using CameraObjects instead of Camera's. should work the same.

enum CameraName
{
	CAMERA_CULLING,
	CAMERA_OVERVIEW,
	CAMERA_GENERIC,
	NO_CAMERA
};

enum CullResult
{
	CULL_INSIDE,
	CULL_OUTSIDE
};

class CameraObject : public GraphicsObject
{
public:
	CameraObject( CameraName name );
	CameraObject();

	void transform( void );
	void draw( void );
	void setRenderState( void );
	void checkCulling(void);

	    // Setup on single camera
	void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
	void setViewport(const int inX, const int inY, const int width, const int height);
	void setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &pos);
	void setCameraModel(CameraModel * const inModel);

	 // update camera system
	void updateCamera(void);

	// Get the matrices for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	// accessors
	void getPos( Vect &outPos) const;
	void getDir( Vect &outDir ) const;
	void getUp( Vect &outUp ) const;
	void getLookAt( Vect &outLookAt ) const;
	void getRight( Vect &outRight ) const;

	CameraName getName() const;

	void setPos( const Vect &inPos);
	Vect getStartPos();		// have to include
			
	void getFieldOfView( float &Value) const;
	void setFieldOfView( const float Value);

	void getNearDist( float &Value) const;
	void setNearDist( const float Value);


	void getNearTopLeft(Vect &vOut) const;
	void getNearTopRight(Vect &vOut) const;
	void getNearBottomLeft(Vect &vOut) const;
	void getNearBottomRight(Vect &vOut) const;	
	void getFarTopLeft(Vect &vOut) const;
	void getFarTopRight(Vect &vOut) const;
	void getFarBottomLeft(Vect &vOut) const;
	void getFarBottomRight(Vect &vOut) const;

	CullResult CullTest( const Sphere &sphere );

private:  // methods
	void privSetViewState( void );
	void privCalcPlaneHeightWidth( void );
	void privCalcFrustumVerts( void );
	void privCalcFrustumCollisionNormals( void );
	void privUpdateProjectionMatrix( void );
	void privUpdateViewMatrix( void );

private:  // data 

	CameraName name;

	// camera unit vectors (up, dir, right)
	Vect	vUp;
	Vect	vDir;
	Vect	vRight;
	Vect	vPos;    
	Vect	vLookAt;

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

	// Projection Matrix
	Matrix projMatrix;
	Matrix viewMatrix;

	CameraModel *model;
};

#endif