
#include "AnimNode.h"
#include <Windows.h>

AnimNode::AnimNode( const char * const inName, const int inBones )
{
	if(strlen(inName) < 32)	{
		memcpy(this->animName, inName, strlen(inName));
		this->animName[strlen(inName)] = '\0';
	}
	else	{
		memcpy(this->animName, inName, 32);
		this->animName[32] = '\0';
	}
	next = 0;
	prev = 0;
	numKeyframes = 0;
	numBones = inBones;
	animData = new Frame_Bucket;
	animData->nextBucket = 0;
	animData->prevBucket = 0;
	animData->pBone = new Bone[numBones];
}

AnimNode::AnimNode()
{ }

AnimNode::~AnimNode()
{
	delete next;
	delete prev;
	delete animData;
	delete[] &animName;
}

void AnimNode::addBucket( Frame_Bucket * node )
{
	Frame_Bucket *ptr = this->animData;
	// go to last bucket in list
	while(ptr->nextBucket != 0)	{
		ptr = ptr->nextBucket;
	}
	node->prevBucket = ptr;
	ptr->nextBucket = node;
	node->nextBucket = 0;
};

char* AnimNode::getName()
{
	return this->animName;
};

Frame_Bucket* AnimNode::getData()
{
	return this->animData;
};