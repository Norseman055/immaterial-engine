#pragma once

#include "AnimNode.h"

class AnimationMan
{
public:
	static void LoadAnimationBuffer( const unsigned char* const, const AnimFileHdr& );
	static AnimNode* GetAnimation( const char * const );
	static void DeleteAnimations();
	
private:
	AnimationMan();
	static AnimationMan* privGetInstance();
	const void privAddToFront(AnimNode* const inNode, AnimNode*& head) const;
	
private:
	// data
	AnimNode *animList;
};