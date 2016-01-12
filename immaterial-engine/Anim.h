
#ifndef ANIM_H
#define ANIM_H

#include "Timer.h"
#include "Vect.h"
#include "Quat.h"

// forward declare
class GraphicsObject;

#define NUM_BONES 80

struct Bone : public Align16
{
	Vect T;
	Quat Q;
	Vect S;

	Bone()
	{
		T.set(0,0,0);
		Q.set(IDENTITY);
		S.set(0,0,0);
	}
};

struct Frame_Bucket
{
	Frame_Bucket	*nextBucket;
	Frame_Bucket	*prevBucket;
	Time			KeyTime;
	Bone			*pBone;

	Frame_Bucket()
	{
		nextBucket = 0;
		prevBucket = 0;
		pBone = new Bone;
	}
};

struct AnimFileHdr
{
	char animName[32];
	int numBones;
	int numKeyframes;

	AnimFileHdr()
		: numBones(0), numKeyframes(0)
	{ }
};

struct KeyTimeHdr
{
	int KeyTime;
	int msTime;
	Bone bArray[NUM_BONES];

	KeyTimeHdr()
		: KeyTime(0), msTime(0)
	{ }
};

void SetAnimationPose( GraphicsObject * const root, Time tCurr );
void SetAnimationHierarchy();

void SetTeddyHierarchy();
void SetHumanoidHierarchy();

#endif