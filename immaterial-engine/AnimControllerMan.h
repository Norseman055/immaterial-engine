#pragma once

#include "AnimController.h"

#define BONE_WIDTH 2.0f

class GraphicsObject;

class AnimControllerMan {
public:
	static void AddController( AnimController* const );
	static void ProcessAnimation( Time &tCurr );
	static void SetAnimationPose( Time &tCurr );
	static void DeleteControllers();

private:
	AnimControllerMan();
	static AnimControllerMan* privGetInstance();
	void privAddToFront( AnimController *inNode, AnimController* &head ) const;

	// data
	int currIndex;
	AnimController *controlList;
};