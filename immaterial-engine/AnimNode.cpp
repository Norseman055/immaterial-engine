
#include "AnimNode.h"
#include "DEBUGGING.h"

AnimNode::AnimNode( const char * const inName, const int inFrames, const int inBones )
	: numKeyframes(inFrames), numBones(inBones), next(nullptr), prev(nullptr), animData(nullptr)
{
	if (strlen(inName) < ANIM_ASSET_NAME_SIZE)	{
		memcpy(this->animName, inName, strlen(inName));
		this->animName[strlen(inName)] = '\0';
	} else {
		memcpy(this->animName, inName, ANIM_ASSET_NAME_SIZE - 1);
		this->animName[ANIM_ASSET_NAME_SIZE - 1] = '\0';
	}
}

AnimNode::AnimNode()
	: numKeyframes(0), numBones(0), next(nullptr), prev(nullptr), animData(nullptr)
{ }

const void AnimNode::addBucket( Frame_Bucket * node )
{
	assert(node != nullptr);

	if (this->animData == nullptr) {
		this->animData = node;
	} else {
		auto ptr = this->animData;
		// go to last bucket in list
		while (ptr->nextBucket != nullptr)	{
			ptr = ptr->nextBucket;
		}
		node->prevBucket = ptr;
		ptr->nextBucket = node;
	}
};

char* AnimNode::getName()
{
	return this->animName;
};

Frame_Bucket* AnimNode::getData() const
{
	return this->animData;
};