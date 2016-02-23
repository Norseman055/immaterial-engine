#include "AnimControllerMan.h"
#include <assert.h>

void AnimControllerMan::AddController( AnimController* const inControl ) {
	auto acMan = privGetInstance();

	inControl->setIndex( acMan->currIndex );
	acMan->currIndex++;

	acMan->privAddToFront( inControl, acMan->controlList );
}

void AnimControllerMan::DeleteControllers() {
	auto walker = privGetInstance()->controlList;
	auto tmp = walker;
	while ( walker != nullptr ) {
		walker = walker->next;
		delete tmp;
		tmp = walker;
	}
}

void AnimControllerMan::ProcessAnimation( Time &tCurr ) {
	auto walker = privGetInstance()->controlList;
	while ( walker != nullptr ) {
		walker->processAnimation( tCurr );
		walker = walker->next;
	}
}

void AnimControllerMan::SetAnimationPose( Time &tCurr ) {
	ProcessAnimation( tCurr );

	auto walker = privGetInstance()->controlList;
	while ( walker != nullptr ) {
		walker->walkAnimNode();
		walker = walker->next;
	}
}

void AnimControllerMan::privAddToFront( AnimController *node, AnimController *& head ) const {
	assert( node != nullptr );

	if ( head == nullptr ) {
		// empty list
		head = node;
	} else {
		// non-empty list, add to front
		node->next = head;
		head->prev = node;
		head = node;
	}
}

AnimControllerMan* AnimControllerMan::privGetInstance() {
	static AnimControllerMan acMan;
	return &acMan;
}

AnimControllerMan::AnimControllerMan()
	: currIndex( 0 ), controlList( nullptr ) { }