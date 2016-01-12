
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CameraNode.h"

CameraNode::CameraNode()
{
	this->myCamera = 0;
}

void CameraNode::set( CameraObject * inCamera )
{
	this->myCamera = inCamera;
}