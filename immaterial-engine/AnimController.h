#pragma once

#include "AnimationManager.h"
#include "Time.h"
#include "PyramidObject.h"

enum PlayMode
{
	Loop = 0,
	Rewind, 
	Clamp,
	Pause
};

class AnimController
{
public:
	AnimController(AnimNode *, PyramidObject *);

	const void processAnimation( Time & ) const;
	const void setIndex( const int );
	const void setPlayback( const PlayMode );
	const void walkAnimNode() const;

	PyramidObject *getSkeleton() const;

private:
	AnimController();

	const void switchTime(Time &) const;
	const void findMaxTime(Time &) const;
	const void setBonePose(PyramidObject * const) const;
	const void privWalkAnimNode(PyramidObject * const) const;
	const void setSkeletonController() const;

private:
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