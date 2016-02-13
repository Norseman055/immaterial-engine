#pragma once

#include "AnimNode.h"

class AnimationMan {
public:
	static void LoadAnimationBuffer( unsigned char* const, const AnimFileHdr& );
	static AnimNode* GetAnimation( const char * const );
	static void DeleteAnimations();

private:
	AnimationMan();
	static AnimationMan* privGetInstance();
	void privAddToFront( AnimNode* const inNode, AnimNode*& head ) const;

	// data
	AnimNode *animList;
};