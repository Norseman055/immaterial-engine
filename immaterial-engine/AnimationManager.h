
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "MathEngine.h"
#include "Anim.h"
#include "AnimNode.h"

class AnimationMan
{
public:
	static void LoadAnimationBuffer( const unsigned char * const animBuff, const AnimFileHdr &aHdr );
	static AnimNode* GetAnimation( const char * const animName );
	static void DeleteAnimations();
	
private:
	AnimationMan();
	static AnimationMan *privGetInstance();
	void privAddToFront(AnimNode *inNode, AnimNode *& head);
	
	// data
	AnimNode *animList;
};

#endif