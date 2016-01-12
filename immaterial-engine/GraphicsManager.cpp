
#include "DEBUGGING.h"

#include "NullObject.h"
#include "PCSNode.h"
#include "GraphicsObject.h"
#include "GraphicsManager.h"
#include "ChunkHeader.h"
#include "eat.h"
#include "PackageHeader.h"

GraphicsObjMan::GraphicsObjMan()
{
   // Get Node 
   PCSNode  *root = this->goTree.getRoot();
   PCSNode  *cullRoot = this->goCull.getRoot();

   // This should be the first time initializing it.
   assert(root == 0 );

   // Create a Dummy root object, it does nothing
   // remember you need to remove this in the destructor
   // gets tricky with singletons
   NullObject *nodeRoot = new NullObject("Root");
   NullObject *cullNode = new NullObject("CullingRoot");

   // insert it into tree
   this->goTree.insert( nodeRoot, root );
   this->goCull.insert( cullNode, cullRoot );

   this->debugOn = false;
}

void GraphicsObjMan::DeleteGraphicsObjects()
{
	GraphicsObjMan* goMan = privGetInstance();
	goMan->goTree.removeDown(goMan->goTree.getRoot());
	goMan->goCull.removeDown(goMan->goCull.getRoot());
}

GraphicsObject * GraphicsObjMan::FindByLocation(Vect &inPos)
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();
	return pGOM->privFindAtLocation( (GraphicsObject *)pGOM->goTree.getRoot()->getChild(), inPos );
}

GraphicsObject * GraphicsObjMan::privFindAtLocation( GraphicsObject *p, Vect &inPos )
{
	GraphicsObject * retObj = 0;
	GraphicsObject * child = 0;

	if(!(p->getStartPos().isEqual(inPos, 0.001f)))
	{
		if (p->getSibling() != 0)
		{
			child = (GraphicsObject *)p->getSibling();
			while(child != 0 && retObj == 0)
			{
				retObj = privFindAtLocation(child, inPos);
				child = (GraphicsObject *)child->getSibling();
			}
		}
	}
	else
	{
		retObj = p;
	}

	return retObj;
}

GraphicsObject * GraphicsObjMan::GetFirstObj()
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();
	
	return (GraphicsObject *)pGOM->goTree.getRoot()->getChild();
}

void GraphicsObjMan::addObject(GraphicsObject *p)
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();

	// Get Node 
	PCSNode *root = pGOM->goTree.getRoot();

	// This should NOT be the first time using the tree.
	assert (root != 0);

	// insert it into tree
	pGOM->goTree.insert(p, root);
}

void GraphicsObjMan::addDebugObject( GraphicsObject *p)
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();

	// get node
	PCSNode *root = pGOM->goCull.getRoot();

	// should not be first time using the tree.
	assert (root != 0);

	// insert it into tree
	pGOM->goCull.insert(p, root);
}

void GraphicsObjMan::drawObjects()
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();

	// Get Node 
	GraphicsObject *root = (GraphicsObject *)pGOM->goTree.getRoot();
	pGOM->privDrawObjectsDepthFirst( root );

	if (pGOM->debugOn)
	{
		root = (GraphicsObject *)pGOM->goCull.getRoot();
		pGOM->privDrawObjectsDepthFirst( root );
	}
}

GraphicsObjMan * GraphicsObjMan::privGetInstance( void )
{
	// This is where its actually stored (BSS section)
	static GraphicsObjMan gom;
	return &gom;
}

void GraphicsObjMan::privDrawObjectsDepthFirst( GraphicsObject *node ) const
{
	GraphicsObject *child = 0;

   // --------- Do draw stuff here -----------------------
   
	//node->dumpNode();
	node->transform();
	node->checkCulling();
	node->setRenderState();
	node->draw();
   
   // --------- Do draw stuff here -----------------------

	// iterate through all of the active children 
	if (node->getChild() != 0)
	{  
		child =	(GraphicsObject *)node->getChild();
		// make sure that allocation is not a child node 
		while (child != 0)
		{
			privDrawObjectsDepthFirst( child );
			child = (GraphicsObject *)child->getSibling();
		}
	}
}

void GraphicsObjMan::DebugSwitch()
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();
	if (pGOM->debugOn)
	{
		pGOM->debugOn = false;
	}
	else
	{
		pGOM->debugOn = true;
	}
}

PCSTree* GraphicsObjMan::getMainTree()
{
	GraphicsObjMan *pGOM = GraphicsObjMan::privGetInstance();

	return &pGOM->goTree;
}