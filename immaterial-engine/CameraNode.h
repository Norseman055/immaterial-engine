
#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

class CameraObject;

class CameraNodeLink
{
public:
	CameraNodeLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	CameraNodeLink *next;
	CameraNodeLink *prev;
};

class CameraNode : public CameraNodeLink
{
public:
	CameraNode();
	void set( CameraObject *inCamera );

public: 
	CameraObject *myCamera;
};

#endif