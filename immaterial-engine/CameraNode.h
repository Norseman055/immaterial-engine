#pragma once

class CameraObject;

class CameraNodeLink {
public:
	CameraNodeLink() 
		: next(nullptr), prev(nullptr)
	{ }

	CameraNodeLink *next;
	CameraNodeLink *prev;
};

class CameraNode : public CameraNodeLink {
public:
	CameraNode();
	const void set( CameraObject* const );

public: 
	CameraObject *myCamera;
};