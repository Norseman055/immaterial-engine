
#include "AnimController.h"
#include "AnimControllerMan.h"
#include "PCSNode.h"
#include "GraphicsManager.h"

AnimController::~AnimController()
{
	delete next;
	delete prev;
	delete result;
	delete animBucket;
	delete skeletonRoot;
}

void AnimController::findMaxTime( Time &tMax )
{
	AnimNode *node = this->animBucket;
	Frame_Bucket *pTmp = node->getData();
	while(pTmp->nextBucket != 0)	{
		pTmp = pTmp->nextBucket;
	}
	tMax = pTmp->KeyTime;
};

void AnimController::switchTime( Time &tCurr )
{
	Time deltaTime = 0.2f * Time(TIME_NTSC_30_FRAME);

	Time maxTime;
	this->findMaxTime(maxTime);

	switch(this->pMode)
	{
	case Loop:
		tCurr += deltaTime;
		if( tCurr < Time(TIME_ZERO) )	{
			tCurr = maxTime;	}
		else if ( tCurr > maxTime )	{
			tCurr = Time(TIME_ZERO);	}
		else	{
			// do nothing
		}
		break;
	case Clamp:
		tCurr += deltaTime;
		if( tCurr < Time(TIME_ZERO) )	{
			tCurr = Time(TIME_ZERO);	}
		else if ( tCurr > maxTime )	{
			tCurr = maxTime;	}
		else	{
			// do nothing
		}
		break;
	case Rewind:
		tCurr -= deltaTime;
		if(tCurr < Time(TIME_ZERO))	{
			tCurr = maxTime;	}
		else if (tCurr > maxTime)	{
			tCurr = Time(TIME_ZERO);	}
		else	{
			// do nothing
		}
		break;
	case Pause:
		// do not change current time.
		if (tCurr < Time(TIME_ZERO))	{
			tCurr = Time(TIME_ZERO);	}
		else if (tCurr > maxTime )	{
			tCurr = maxTime;	}
		else	{
			// do nothing
		}
		break;
	}
}

void AnimController::processAnimation( Time &tCurr )
{
	switchTime(tCurr);

	AnimNode *node = this->animBucket;
	Frame_Bucket* pTmp = node->getData();
	pTmp = pTmp->nextBucket;

	Bone *bResult = this->result->pBone;

	while( tCurr >= pTmp->KeyTime && pTmp->nextBucket != 0 )
	{
		pTmp = pTmp->nextBucket;
	}

	// pTmp is the "B" key frame
	// pTmp->prev is the "A" key frame
	Frame_Bucket *pA = pTmp->prevBucket;
	Frame_Bucket *pB = pTmp;

	// find the "S" of the time
	float tS = (tCurr - pA->KeyTime)  /(pB->KeyTime - pA->KeyTime);
	
	// interpolate to 
	Bone *bA = pA->pBone;
	Bone *bB = pB->pBone;

   // Interpolate to tS time, for all bones
	for( int i = 0; i < node->numBones; i++ )
	{
		// interpolate ahoy!
		VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
		QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
		VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
		
		// advance the pointer
		bA++;
		bB++;
		bResult++;
	}
}

void AnimController::setPlayback( PlayMode inMode )
{
	this->pMode = inMode;
}

void AnimController::walk_anim_node()
{
	this->priv_walk_anim_node(this->skeletonRoot);
}

void AnimController::priv_walk_anim_node( PyramidObject *node )
{
	setBonePose( node );

	PyramidObject *child = 0;

	if (node->getChild() != 0)	{
		child = (PyramidObject *)node->getChild();
		while (child != 0)	{
			priv_walk_anim_node( child );
			child = (PyramidObject *)child->getSibling();
		}
	}
}

void AnimController::setBonePose( PyramidObject *node )
{
	PyramidObject *childNode = (PyramidObject *)node;
	PyramidObject *parentNode = (PyramidObject *)node->getParent();

	if( parentNode == GraphicsObjMan::getMainTree()->getRoot() )
		return;

	if (parentNode != 0 && childNode != 0)	{
		// Now get the world matrices
		Vect start(0.0f,0.0f,0.0f);

		parentNode->transform();
		Vect ptA = start * parentNode->getWorld();

		childNode->transform();
		Vect ptB = start * childNode->getWorld();

		// direction between the anchor points of the respective bones
		Vect dir = -(ptB - ptA);

		// length of the bone 0
		float mag = dir.mag();

		// Set the orientation and length for bone 0
		Matrix S( SCALE, BONE_WIDTH, BONE_WIDTH, mag);
		Quat Q( ROT_ORIENT, dir.getNorm(), Vect( 0.0f, 1.0f, 0.0f) );
		Matrix T( TRANS, ptB );

		Matrix BoneOrient = S * Q * T;

		childNode->setBoneOrientation( BoneOrient );
	}
}

void AnimController::setIndex( const int _index )
{
	this->index = _index;
}

PyramidObject* AnimController::getSkeleton()
{
	return this->skeletonRoot;
}

AnimController::AnimController( AnimNode *inBucket, PyramidObject *inRoot )
{
	this->next = 0;
	this->prev = 0;
	this->pMode = Loop;
	this->index = -1;
	this->animBucket = inBucket;
	
	this->skeletonRoot = inRoot;
	setSkeletonController();

	this->result = new Frame_Bucket;
}

void AnimController::setSkeletonController( )
{
	PyramidObject *walker = this->skeletonRoot;
	walker->setControllerDepthFirst( this );
}

AnimController::AnimController()
{ }