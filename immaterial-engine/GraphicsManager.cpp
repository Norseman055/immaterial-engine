
#include "DEBUGGING.h"

#include "NullObject.h"
#include "GraphicsManager.h"
#include "ChunkHeader.h"
#include "eat.h"
#include "PackageHeader.h"

GraphicsObjMan::GraphicsObjMan() 
	: debugOn(false)
{
	// insert roots into trees
	this->goTree.insert( new NullObject( "Root" ), this->goTree.getRoot() );
	this->goCull.insert( new NullObject( "CullingRoot" ), this->goCull.getRoot() );
}

void GraphicsObjMan::DeleteGraphicsObjects() {
	auto goMan = privGetInstance();
	goMan->goTree.removeDown(goMan->goTree.getRoot());
	goMan->goCull.removeDown(goMan->goCull.getRoot());
}

GraphicsObject* GraphicsObjMan::FindByLocation(Vect &inPos) {
	auto pGOM = GraphicsObjMan::privGetInstance();
	return pGOM->privFindAtLocation( (GraphicsObject *)pGOM->goTree.getRoot()->getChild(), inPos );
}

GraphicsObject* GraphicsObjMan::privFindAtLocation( GraphicsObject* const p, Vect& inPos ) {
	GraphicsObject* retObj = nullptr;
	GraphicsObject* child = 0;

	if( !(p->getStartPos().isEqual(inPos, 0.001f)) ) {
		if( p->getSibling() != 0 ) {
			child = (GraphicsObject *)p->getSibling();
			while(child != 0 && retObj == nullptr) {
				retObj = privFindAtLocation(child, inPos);
				child = (GraphicsObject *)child->getSibling();
			}
		}
	} else {
		retObj = p;
	}

	return retObj;
}

GraphicsObject* GraphicsObjMan::GetFirstObj() {
	return ( GraphicsObject * ) GraphicsObjMan::privGetInstance()->goTree.getRoot()->getChild();
}

void GraphicsObjMan::AddObject(GraphicsObject* const p) {
	auto pGOM = GraphicsObjMan::privGetInstance();
	auto root = pGOM->goTree.getRoot();

	assert (root != 0);

	pGOM->goTree.insert(p, root);
}

void GraphicsObjMan::AddDebugObject( GraphicsObject* const p) {
	auto pGOM = GraphicsObjMan::privGetInstance();
	auto root = pGOM->goCull.getRoot();

	assert (root != 0);

	pGOM->goCull.insert(p, root);
}

void GraphicsObjMan::DrawObjects() {
	auto pGOM = GraphicsObjMan::privGetInstance();
	auto root = (GraphicsObject *)pGOM->goTree.getRoot();

	pGOM->privDrawObjectsDepthFirst( root );
	if (pGOM->debugOn) {
		root = (GraphicsObject *)pGOM->goCull.getRoot();
		pGOM->privDrawObjectsDepthFirst( root );
	}
}

GraphicsObjMan* GraphicsObjMan::privGetInstance() {
	static GraphicsObjMan gom;
	return &gom;
}

const void GraphicsObjMan::privDrawObjectsDepthFirst( GraphicsObject* const node ) const {
	GraphicsObject *child = 0;

	node->transform();
	node->checkCulling();
	node->setRenderState();
	node->draw();

	if (node->getChild() != 0) {  
		child =	(GraphicsObject *)node->getChild();
		while (child != 0) {
			privDrawObjectsDepthFirst( child );
			child = (GraphicsObject *)child->getSibling();
		}
	}
}

void GraphicsObjMan::DebugSwitch() {
	auto pGOM = GraphicsObjMan::privGetInstance();
	if (pGOM->debugOn) {
		pGOM->debugOn = false;
	} else {
		pGOM->debugOn = true;
	}
}

PCSTree* GraphicsObjMan::GetMainTree() {
	auto pGOM = GraphicsObjMan::privGetInstance();
	return &pGOM->goTree;
}