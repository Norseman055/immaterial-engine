
#ifndef ANIM_NODE_H
#define ANIM_NODE_H

#include "Anim.h"

#define ANIM_ASSET_NAME_SIZE 32

class AnimNode
{
public:
	AnimNode(const char * const, const int, const int);

	const void addBucket( Frame_Bucket * );
	char* getName();
	Frame_Bucket* getData() const;

private:
	AnimNode();
	
public:
	// data links
	int numKeyframes;
	int numBones;
	AnimNode* next;
	AnimNode* prev;

private:
	// data
	Frame_Bucket *animData;
	char animName[32];
};

#endif