
#include "AnimControllerMan.h"
#include "AnimController.h"
#include "Time.h"
#include "GraphicsManager.h"
#include <assert.h>

void AnimControllerMan::AddController( AnimController * inControl )
{
	AnimControllerMan* acMan = privGetInstance();
	
	inControl->setIndex(acMan->currIndex);
	acMan->currIndex++;

	acMan->privAddToFront( inControl, acMan->controlList );
}

void AnimControllerMan::DeleteControllers()
{
	AnimControllerMan* acMan = privGetInstance();
	AnimController* walker = acMan->controlList;
	AnimController* tmp = walker;
	while (walker != 0)	{
		walker = walker->next;
		delete tmp;
		tmp = walker;
	}
}

void AnimControllerMan::ProcessAnimation( Time &tCurr )
{
	AnimControllerMan* acMan = privGetInstance();

	AnimController *walker = acMan->controlList;
	while (walker != 0)	{
		walker->processAnimation(tCurr);
		walker = walker->next;
	}
}

void AnimControllerMan::SetAnimationPose( Time &tCurr )
{
	AnimControllerMan* acMan = privGetInstance();
	ProcessAnimation( tCurr );

	AnimController *walker = acMan->controlList;
	while (walker != 0) {
		walker->walk_anim_node();
		walker = walker->next;
	}
}

void AnimControllerMan::privAddToFront(AnimController *node, AnimController *&head)
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
}

AnimControllerMan* AnimControllerMan::privGetInstance()
{
	static AnimControllerMan acMan;
	return &acMan;
}

AnimControllerMan::AnimControllerMan()
{
	this->controlList = 0;
	this->currIndex = 0;
}