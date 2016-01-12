
#include "AnimationManager.h"
#include <assert.h>
#include <Windows.h>

void AnimationMan::LoadAnimationBuffer(const unsigned char * const animBuff, const AnimFileHdr &aHdr )
{
	AnimationMan* aMan = privGetInstance();

	AnimNode *node = new AnimNode(aHdr.animName, aHdr.numBones);
	node->numKeyframes = aHdr.numKeyframes;

	// set pointers to first keyframe data and bone data
	KeyTimeHdr *KTHdr = (KeyTimeHdr *)animBuff;
	Bone *ptr = KTHdr->bArray;

	// now have an animNode with the proper data, now i need to create the frame buckets for the animation
	Frame_Bucket *tmp = 0;
	for (int i = 0; i < aHdr.numKeyframes; i++)	{
		// make new frame bucket, add data
		tmp = new Frame_Bucket;
		tmp->KeyTime = KTHdr->KeyTime * Time(TIME_NTSC_30_FRAME);

		// copy bones
		tmp->pBone = (Bone *)malloc(sizeof(Bone) * aHdr.numBones);
		for (int j = 0; j < aHdr.numBones; j++)	{
			tmp->pBone[j] = ptr[j];
		}

		node->addBucket(tmp);

		// advance pointers
		KTHdr = (KeyTimeHdr*)((unsigned int)ptr + (sizeof(Bone) * aHdr.numBones));
		ptr = KTHdr->bArray;
	}

	aMan->privAddToFront(node, aMan->animList);
};

void AnimationMan::privAddToFront( AnimNode * node, AnimNode *&head)
{
	assert (node != 0);

	// empty list
	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else	// non-empty list, add to front
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
};

AnimNode* AnimationMan::GetAnimation( const char * const inName )
{
	AnimationMan *aMan = privGetInstance();
	AnimNode *walker = aMan->animList;
	while (walker->next != 0)	{
		if (strcmp(walker->getName(), inName) == 0)	{
			break;
		}
		walker = walker->next;
	}

	return walker;
};

AnimationMan* AnimationMan::privGetInstance()
{
	static AnimationMan aMan;
	return &aMan;
};

AnimationMan::AnimationMan()
{
	this->animList = 0;
};

void AnimationMan::DeleteAnimations()
{
	AnimationMan *aMan = privGetInstance();
	AnimNode *walker = aMan->animList;
	AnimNode *tmp = walker;

	while(walker != 0)	{
		walker = walker->next;
		tmp->~AnimNode();
		tmp = walker;
	}
}