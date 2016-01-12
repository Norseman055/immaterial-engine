
#ifndef ANIM_NODE_H
#define ANIM_NODE_H

#include "Anim.h"

class AnimNode
{
public:
	AnimNode(const char * const inName, const int inBones);
	~AnimNode();

	void addBucket( Frame_Bucket * inFB );
	char* getName();
	Frame_Bucket* getData();

	// data links
	AnimNode *next;
	AnimNode *prev;
	int numKeyframes;
	int numBones;

private:
	AnimNode();

	// data
	char animName[32];
	Frame_Bucket *animData;
};

#endif