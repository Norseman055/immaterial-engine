#pragma once

#include "Timer.h"
#include "MathEngine.h"

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
	Time			KeyTime;
	Frame_Bucket	*nextBucket;
	Frame_Bucket	*prevBucket;
	Bone			*pBone;

	Frame_Bucket()
		: nextBucket(nullptr), prevBucket(nullptr), pBone(nullptr)
	{ }
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
	Bone bArray[NUM_BONES];
	int KeyTime;
	int msTime;

	KeyTimeHdr()
		: KeyTime(0), msTime(0)
	{ }
};

void SetAnimationPose( GraphicsObject * const root, Time tCurr );
void SetAnimationHierarchy();

void SetTeddyHierarchy();
void SetHumanoidHierarchy();