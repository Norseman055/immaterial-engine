#pragma once

#include "AnimationManager.h"
#include "Time.h"
#include "PyramidObject.h"

enum PlayMode {
	Loop = 0,
	Rewind,
	Clamp,
	Pause
};

class AnimController {
public:
	AnimController( AnimNode *, PyramidObject * );

	void processAnimation( Time & ) const;
	void setIndex( const int );
	void setPlayback( const PlayMode );
	void walkAnimNode() const;

	PyramidObject *getSkeleton() const;

private:
	AnimController();

	void switchTime( Time & ) const;
	void findMaxTime( Time & ) const;
	void setBonePose( PyramidObject * const ) const;
	void privWalkAnimNode( PyramidObject * const ) const;
	void setSkeletonController() const;

	// data
	int index;
	PlayMode pMode;
	AnimNode *animBucket;
	PyramidObject *skeletonRoot;

public:
	AnimController *next;
	AnimController *prev;
	Frame_Bucket *result;
};