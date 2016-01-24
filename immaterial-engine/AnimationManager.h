
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

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
	
private:
	// data
	AnimNode *animList;
};

#endif