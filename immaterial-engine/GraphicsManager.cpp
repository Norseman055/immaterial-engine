#include "DEBUGGING.h"

#include "NullObject.h"
#include "GraphicsManager.h"

GraphicsObjMan::GraphicsObjMan()
	: debugOn( false ) {
	// insert roots into trees
	this->goTree.insert( new NullObject( "Root" ), this->goTree.getRoot() );
	this->goCull.insert( new NullObject( "CullingRoot" ), this->goCull.getRoot() );
}

void GraphicsObjMan::DeleteGraphicsObjects() {
	auto goMan = privGetInstance();
	goMan->goTree.removeDown( goMan->goTree.getRoot() );
	goMan->goCull.removeDown( goMan->goCull.getRoot() );
}

GraphicsObject* GraphicsObjMan::FindByLocation( Vect &inPos ) {
	auto pGOM = privGetInstance();
	return pGOM->privFindAtLocation( static_cast< GraphicsObject * >(pGOM->goTree.getRoot()->getChild()), inPos );
}

GraphicsObject* GraphicsObjMan::privFindAtLocation( GraphicsObject* const p, Vect& inPos ) const {
	GraphicsObject* retObj = nullptr;
	GraphicsObject* child;

	if ( !(p->getStartPos().isEqual( inPos, 0.001f )) ) {
		if ( p->getSibling() != nullptr ) {
			child = static_cast< GraphicsObject * >(p->getSibling());
			while ( child != nullptr && retObj == nullptr ) {
				retObj = privFindAtLocation( child, inPos );
				child = static_cast< GraphicsObject * >(child->getSibling());
			}
		}
	} else {
		retObj = p;
	}

	return retObj;
}

GraphicsObject* GraphicsObjMan::GetFirstObj() {
	return static_cast< GraphicsObject * >(privGetInstance()->goTree.getRoot()->getChild());
}

void GraphicsObjMan::AddObject( GraphicsObject* const p ) {
	auto pGOM = privGetInstance();
	auto root = pGOM->goTree.getRoot();

	assert( root );

	pGOM->goTree.insert( p, root );
}

void GraphicsObjMan::AddDebugObject( GraphicsObject* const p ) {
	auto pGOM = privGetInstance();
	auto root = pGOM->goCull.getRoot();

	assert( root );

	pGOM->goCull.insert( p, root );
}

void GraphicsObjMan::DrawObjects() {
	auto pGOM = privGetInstance();
	auto root = static_cast< GraphicsObject * >(pGOM->goTree.getRoot());

	pGOM->privDrawObjectsDepthFirst( root );
	if ( pGOM->debugOn ) {
		root = static_cast< GraphicsObject * >(pGOM->goCull.getRoot());
		pGOM->privDrawObjectsDepthFirst( root );
	}
}

GraphicsObjMan* GraphicsObjMan::privGetInstance() {
	static GraphicsObjMan gom;
	return &gom;
}

void GraphicsObjMan::privDrawObjectsDepthFirst( GraphicsObject* const node ) const {
	GraphicsObject *child;

	node->transform();
	node->checkCulling();
	node->setRenderState();
	node->draw();

	if ( node->getChild() != nullptr ) {
		child = static_cast< GraphicsObject * >(node->getChild());
		while ( child != nullptr ) {
			privDrawObjectsDepthFirst( child );
			child = static_cast< GraphicsObject * >(child->getSibling());
		}
	}
}

void GraphicsObjMan::DebugSwitch() {
	auto pGOM = privGetInstance();
	if ( pGOM->debugOn ) {
		pGOM->debugOn = false;
	} else {
		pGOM->debugOn = true;
	}
}

PCSTree* GraphicsObjMan::GetMainTree() {
	auto pGOM = privGetInstance();
	return &pGOM->goTree;
}