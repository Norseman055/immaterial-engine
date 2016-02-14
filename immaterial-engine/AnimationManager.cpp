#include "AnimationManager.h"
#include "DEBUGGING.h"

void AnimationMan::LoadAnimationBuffer( unsigned char* const animBuff, const AnimFileHdr& aHdr ) {
	auto aMan = privGetInstance();

	auto node = new AnimNode( aHdr.animName, aHdr.numKeyframes, aHdr.numBones );

	// set pointers to first keyframe data and bone data
	auto KTHdr = reinterpret_cast< KeyTimeHdr * >(animBuff);
	auto ptr = KTHdr->bArray;

	// now have an animNode with the proper data, now i need to create the frame buckets for the animation
	Frame_Bucket *tmp;
	for ( auto i = 0; i < aHdr.numKeyframes; i++ ) {
		// make new frame bucket, add data
		tmp = new Frame_Bucket;
		tmp->KeyTime = KTHdr->KeyTime * Time( TIME_NTSC_30_FRAME );

		// copy bones
		tmp->pBone = new Bone[aHdr.numBones];
		for ( auto j = 0; j < aHdr.numBones; j++ ) {
			tmp->pBone[j] = ptr[j];
		}

		node->addBucket( tmp );

		// advance pointers
		KTHdr = reinterpret_cast< KeyTimeHdr* >(reinterpret_cast< unsigned int >( ptr ) +(sizeof( Bone ) * aHdr.numBones));
		ptr = KTHdr->bArray;
	}

	aMan->privAddToFront( node, aMan->animList );
};

void AnimationMan::privAddToFront( AnimNode* const node, AnimNode*& head ) const {
	assert( node );

	// empty list
	if ( head == nullptr ) {
		head = node;
	} else	// non-empty list, add to front
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
};

AnimNode* AnimationMan::GetAnimation( const char* const inName ) {
	auto walker = privGetInstance()->animList;
	while ( walker->next != nullptr ) {
		if ( strcmp( walker->getName(), inName ) == 0 ) {
			break;
		}
		walker = walker->next;
	}

	return walker;
};

AnimationMan* AnimationMan::privGetInstance() {
	static AnimationMan aMan;
	return &aMan;
};

AnimationMan::AnimationMan()
	: animList( nullptr ) { }

void AnimationMan::DeleteAnimations() {
	auto aMan = privGetInstance();
	auto walker = aMan->animList;
	auto tmp = walker;

	while ( walker != nullptr ) {
		walker = walker->next;
		delete tmp;
		tmp = walker;
	}
}