
#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "Anim.h"
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
	~AnimController();

	void processAnimation( Time &tCurr );
	AnimController( AnimNode *inBucket, PyramidObject *inRoot);
	void setIndex( const int _index );
	void setPlayback( PlayMode inMode );
	void walk_anim_node();
	PyramidObject *getSkeleton();

	AnimController *next;
	AnimController *prev;
	Frame_Bucket *result;

private:
	void switchTime( Time &tCurr );
	void findMaxTime( Time &outTime );
	void setBonePose(PyramidObject *node);
	void priv_walk_anim_node(PyramidObject *node);
	void setSkeletonController( );
	AnimController();
	
	// data
	int index;
	PlayMode pMode;
	AnimNode *animBucket;
	PyramidObject *skeletonRoot;
};

#endif