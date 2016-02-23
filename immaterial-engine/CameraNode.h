#pragma once

#include "Camera.h"
#include "NodeLink.h"

class CameraNode : public NodeLink<Camera> {
public:
	CameraNode( Camera* const inCamera )
		: NodeLink( inCamera ) { }
};