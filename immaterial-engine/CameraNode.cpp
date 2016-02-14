#include "OpenGL.h"
#include "DEBUGGING.h"

#include "CameraNode.h"

CameraNode::CameraNode()
	: myCamera( nullptr ) { }

void CameraNode::set( CameraObject* const inCamera ) {
	this->myCamera = inCamera;
}