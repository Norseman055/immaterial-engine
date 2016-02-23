#include "AnimController.h"
#include "AnimControllerMan.h"
#include "GraphicsManager.h"

void AnimController::findMaxTime( Time &tMax ) const {
	auto node = this->animBucket;
	auto pTmp = node->getData();
	while ( pTmp->nextBucket != nullptr ) {
		pTmp = pTmp->nextBucket;
	}
	tMax = pTmp->KeyTime;
};

void AnimController::switchTime( Time &tCurr ) const {
	auto deltaTime = 0.2f * Time( TIME_NTSC_30_FRAME );

	Time maxTime;
	this->findMaxTime( maxTime );

	switch ( this->pMode ) {
		case Loop:
			tCurr += deltaTime;
			if ( tCurr < Time( TIME_ZERO ) ) {
				tCurr = maxTime;
			} else if ( tCurr > maxTime ) {
				tCurr = Time( TIME_ZERO );
			} else {
				// do nothing
			}
			break;
		case Clamp:
			tCurr += deltaTime;
			if ( tCurr < Time( TIME_ZERO ) ) {
				tCurr = Time( TIME_ZERO );
			} else if ( tCurr > maxTime ) {
				tCurr = maxTime;
			} else {
				// do nothing
			}
			break;
		case Rewind:
			tCurr -= deltaTime;
			if ( tCurr < Time( TIME_ZERO ) ) {
				tCurr = maxTime;
			} else if ( tCurr > maxTime ) {
				tCurr = Time( TIME_ZERO );
			} else {
				// do nothing
			}
			break;
		case Pause:
			// do not change current time.
			if ( tCurr < Time( TIME_ZERO ) ) {
				tCurr = Time( TIME_ZERO );
			} else if ( tCurr > maxTime ) {
				tCurr = maxTime;
			} else {
				// do nothing
			}
			break;
	}
}

void AnimController::processAnimation( Time & tCurr ) const {
	switchTime( tCurr );

	auto node = this->animBucket;
	auto pTmp = node->getData();
	pTmp = pTmp->nextBucket;

	auto bResult = this->result->pBone;

	while ( tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr ) {
		pTmp = pTmp->nextBucket;
	}

	// pTmp is the "B" key frame
	// pTmp->prev is the "A" key frame
	auto pA = pTmp->prevBucket;
	auto pB = pTmp;

	// find the "S" of the time
	auto tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

	// interpolate to
	auto bA = pA->pBone;
	auto bB = pB->pBone;

	// Interpolate to tS time, for all bones
	for ( auto i = 0; i < node->numBones; i++ ) {
		// interpolate ahoy!
		VectApp::Lerp( bResult->T, bA->T, bB->T, tS );
		QuatApp::Slerp( bResult->Q, bA->Q, bB->Q, tS );
		VectApp::Lerp( bResult->S, bA->S, bB->S, tS );

		// advance the pointer
		bA++;
		bB++;
		bResult++;
	}
}

void AnimController::setPlayback( const PlayMode inMode ) {
	this->pMode = inMode;
}

void AnimController::walkAnimNode() const {
	this->privWalkAnimNode( this->skeletonRoot );
}

void AnimController::privWalkAnimNode( PyramidObject * const node ) const {
	setBonePose( node );

	PyramidObject *child;

	if ( node->getChild() != nullptr ) {
		child = static_cast< PyramidObject * >(node->getChild());
		while ( child != nullptr ) {
			privWalkAnimNode( child );
			child = static_cast< PyramidObject * >(child->getSibling());
		}
	}
}

void AnimController::setBonePose( PyramidObject * const node ) const {
	auto childNode = node;
	auto parentNode = static_cast< PyramidObject * >(node->getParent());

	if ( parentNode == GraphicsObjMan::GetMainTree()->getRoot() )
		return;

	if ( parentNode != nullptr && childNode != nullptr ) {
		// Now get the world matrices
		Vect start( 0.0f, 0.0f, 0.0f );

		parentNode->transform();
		auto ptA = start * parentNode->getWorld();

		childNode->transform();
		auto ptB = start * childNode->getWorld();

		// direction between the anchor points of the respective bones
		auto dir = -(ptB - ptA);

		// length of the bone 0
		auto mag = dir.mag();

		// Set the orientation and length for bone 0
		Matrix S( SCALE, BONE_WIDTH, BONE_WIDTH, mag );
		Quat Q( ROT_ORIENT, dir.getNorm(), Vect( 0.0f, 1.0f, 0.0f ) );
		Matrix T( TRANS, ptB );

		auto BoneOrient = S * Q * T;

		childNode->setBoneOrientation( BoneOrient );
	}
}

void AnimController::setIndex( const int _index ) {
	this->index = _index;
}

PyramidObject* AnimController::getSkeleton() const {
	return this->skeletonRoot;
}

AnimController::AnimController( AnimNode *inBucket, PyramidObject *inRoot )
	: index( -1 ), pMode( Loop ), animBucket( inBucket ), skeletonRoot( inRoot ), next( nullptr ), prev( nullptr ) {
	setSkeletonController();
	this->result = new Frame_Bucket();
	this->result->pBone = new Bone();
}

void AnimController::setSkeletonController() const {
	auto walker = this->skeletonRoot;
	walker->setControllerDepthFirst( this );
}

AnimController::AnimController()
	: index( 0 ), pMode( Pause ), animBucket( nullptr ), skeletonRoot( nullptr ), next( nullptr ), prev( nullptr ), result( nullptr ) { }