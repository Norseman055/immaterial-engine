
#ifndef ANIM_CONTROLLER_MAN_H
#define ANIM_CONTROLLER_MAN_H

#include "AnimController.h"

#define BONE_WIDTH 2.0f

class GraphicsObject;

class AnimControllerMan
{
public:
	static void AddController( AnimController * inControl );
	static void ProcessAnimation( Time &tCurr );
	static void SetAnimationPose( Time &tCurr );
	static void DeleteControllers();

private:
	AnimControllerMan();
	static AnimControllerMan* privGetInstance();
	void privAddToFront(AnimController *inNode, AnimController* &head);
	void priv_walk_anim_node( GraphicsObject *node );
	void setBonePose( GraphicsObject *node );

	// data
	AnimController *controlList;
	int currIndex;

};

#endif