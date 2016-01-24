
#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CameraNode.h"

CameraNode::CameraNode()
	: myCamera(nullptr)
{ }

const void CameraNode::set( CameraObject* const inCamera ) {
	this->myCamera = inCamera;
}