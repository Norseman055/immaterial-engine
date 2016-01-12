#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "PCSTree.h"
#include "PCSNode.h"


// constructor
PCSTree::PCSTree()
{
	info.maxLevelCount = 0;
	info.maxNodeCount = 0;
	info.numLevels = 0;
	info.numNodes = 0;
	root = NULL;
};

// destructor
PCSTree::~PCSTree()
{
 //  printf("PCSTree: destructor()\n");
};


// get Root
PCSNode *PCSTree::getRoot( void ) const
{
   return (PCSNode *)root;
}


// insert
void PCSTree::insert(PCSNode *inNode, PCSNode *parent)
{
	if (this->getRoot() != 0)				// if tree has a root already
	{
		inNode->setParent(parent);		// set node's parent to specified parent.
		inNode->setLevel((inNode->getParent()->getLevel()) + 1);

		if (parent->getChild() != 0)			// if parent node has children, add it as first sibling on list.
		{	// store first sibling as tmp, add node as parent's child, adjust sibling list
			PCSNode *tmp = parent->getChild();
			parent->setChild(inNode);
			inNode->setSibling(tmp);
		}
		else							// if that parent node has no children, make it a child.
		{
			parent->setChild(inNode);	// set parents node child to inNode
		}
	}

	else								// tree has no root. it is now the root. forget your parent specifier.
	{
		root = inNode;					// make root the first node
		inNode->setParent(NULL);		// set the node's parent to null. its now the root.
		inNode->setLevel(1);
	}

	checkLevels(inNode);
	info.maxNodeCount++;
	info.numNodes++;
}


// Remove
// super easy process. just remove its link from the parent and make sure siblings to left link to the right
void PCSTree::remove(PCSNode * const inNode)
{
	// make sure inNode is NOT the root. if it is the root, just zero out all its stats.
	if (inNode != root)
	{
		// check to see if inNode is NOT the only child.
		if (inNode->getParent()->getChild() != inNode || inNode->getSibling())
		{
			// first case, its a "first child". set the parent's first child to the next child in line.
			if (inNode->getParent()->getChild() == inNode)
			{
				inNode->getParent()->setChild( inNode->getSibling() );
			}

			// second case, its the "middle" or "youngest". find previous sibling point.
			else 
			{
				PCSNode *tmp = inNode->getParent()->getChild();		// create temp node to find previous child
				while (tmp->getSibling() != inNode)
					tmp = tmp->getSibling();						// move temp node to position right before child
			
				// youngest
				if (inNode->getSibling() == NULL)
					tmp->setSibling(NULL);							// set final sibling to null
				// middle
				else
					tmp->setSibling(inNode->getSibling());			// link inNode's previous sibling to its next sibling.
			}
		}

		// inNode is only child. just tell parent that it has no children.
		else
			inNode->getParent()->setChild(NULL);
	}
	else
		root = NULL;

	// set everything derived from inNode (children and their siblings, etc) to 0
	if (inNode->getChild())
		removeDown(inNode->getChild());

	// clean up inNode links to 0
	inNode->setChild(NULL);
	inNode->setParent(NULL);
	inNode->setSibling(NULL);

	// update info
	info.numNodes--;
}

 // get tree info
void PCSTree::getInfo( PCSTreeInfo &infoContainer )
{
	info.numLevels = 0;
	PCSNode *travelTree = root;
	if (travelTree != NULL)				// if it isn't a null pointer, check the depth of the tree.
	{
		goDown(travelTree);
	}

	infoContainer.maxLevelCount = info.maxLevelCount;
	infoContainer.maxNodeCount = info.maxNodeCount;
	infoContainer.numLevels = info.numLevels;
	infoContainer.numNodes = info.numNodes;
}


void PCSTree::dumpTree( ) const
{
	if (root)
	{
		printf("\ndumpTree() ----------------");
		root->printDown(root);
	}
}


// this function is called just once to check the levels with getInfo().
void PCSTree::goDown( const PCSNode* const _root )
{
	if (_root->getChild() != NULL)		// if root has a child, go down again.
	{
		goDown(_root->getChild());
	}

	checkLevels(_root);					// check levels for that tree

	if (_root->getSibling() != NULL)		// now traverse siblings.
	{
		goDown(_root->getSibling());
	}
}

// this deletes all children below a specified original root. basically the same as goDown, but calls remove function.
// at its final iteration, it is always at the final sibling, so it zeroes everything out.
void PCSTree::removeDown( PCSNode * const _root )
{	
	if (_root->getChild() != NULL)		// if root has a child, go down again.
	{
		removeDown(_root->getChild());
	}

	if (_root->getSibling() != NULL)		// now traverse siblings.
	{
		removeDown(_root->getSibling());
	}

	// zero out current node
	_root->setChild(NULL);
	_root->setParent(NULL);
	_root->setSibling(NULL);

	// update info
	info.numNodes--;
}

// this is a function that just checks the number of levels away from the root any given node is.
void PCSTree::checkLevels( const PCSNode * const inNode )
{
	if (inNode->getLevel() > info.maxLevelCount)		// if current level is greater than max, increase max
		info.maxLevelCount = inNode->getLevel();

	if (inNode->getLevel() > info.numLevels)			// if current level is greater than recorded levels, increase recorded levels
		info.numLevels = inNode->getLevel();
}